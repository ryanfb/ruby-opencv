/************************************************************

   cvseq.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvseq.h"
/*
 * Document-class: OpenCV::CvSeq
 *
 * Generic Sequence class. CvSeq has the method like Array (push, pop, select, etc...).
 * But, CvSeq cannot store the object of a different class.
 * When storing object in CvSeq, conversion is automatically tried,
 * and the object occurs the error if it cannot be done.
 *
 * e.g.
 *   seq = CvSeq.new(CvPoint)    # Argument mean "this sequence contain only this class's object"
 *   seq.push(CvPoint.new(0, 0)) # => it's ok
 *   seq.push("hello")           # => try convert "hello" to CvPoint. but can't do it. raise error.
 *
 * If the sequecne contain object of class A.
 * When storing object(named "obj") of class B to the sequence.
 *   Try automatically : A.from_B(obj) => object of class A.
 *
 * The sequence might have another sequence outside. see below. 
 * Each sequece has h_prev, h_next, v_prev, v_next method.
 * If the adjoining sequence exists, each method return the adjoining sequence.
 * Otherwise return nil.
 *
 * link:../images/CvSeq_relationmap.png
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVSEQ

VALUE rb_allocate(VALUE klass);
void cvseq_free(void *ptr);

VALUE rb_klass;
// contain sequence-block class
st_table *seqblock_klass_table = st_init_numtable();

VALUE
rb_class()
{
  return rb_klass;
}

VALUE
seqblock_class(void *ptr)
{
  VALUE klass;
  if (!st_lookup(seqblock_klass_table, (st_data_t)ptr, (st_data_t*)&klass)) {
    rb_raise(rb_eTypeError, "Invalid sequence error.");
  }
  return klass;
}

void
resist_class_information_of_sequence(CvSeq *seq, VALUE klass)
{
  st_insert(seqblock_klass_table, (st_data_t)seq, (st_data_t)klass);
}

void
define_ruby_class()
{
  if(rb_klass)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   * 
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  rb_klass = rb_define_class_under(opencv, "CvSeq", rb_cObject);
  rb_include_module(rb_klass, rb_mEnumerable);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_private_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "total", RUBY_METHOD_FUNC(rb_total), 0);
  rb_define_alias(rb_klass, "length", "total");
  rb_define_alias(rb_klass, "size", "total");
  rb_define_method(rb_klass, "empty?", RUBY_METHOD_FUNC(rb_empty_q), 0);
  rb_define_method(rb_klass, "[]", RUBY_METHOD_FUNC(rb_aref), 1);
  rb_define_method(rb_klass, "first", RUBY_METHOD_FUNC(rb_first), 0);
  rb_define_method(rb_klass, "last", RUBY_METHOD_FUNC(rb_last), 0);
  
  rb_define_method(rb_klass, "h_prev", RUBY_METHOD_FUNC(rb_h_prev), 0);
  rb_define_method(rb_klass, "h_next", RUBY_METHOD_FUNC(rb_h_next), 0);
  rb_define_method(rb_klass, "v_prev", RUBY_METHOD_FUNC(rb_v_prev), 0);
  rb_define_method(rb_klass, "v_next", RUBY_METHOD_FUNC(rb_v_next), 0);

  rb_define_method(rb_klass, "push", RUBY_METHOD_FUNC(rb_push), -2);
  rb_define_alias(rb_klass, "<<", "push");
  rb_define_method(rb_klass, "pop", RUBY_METHOD_FUNC(rb_pop), 0);
  rb_define_method(rb_klass, "unshift", RUBY_METHOD_FUNC(rb_unshift), -2);
  rb_define_alias(rb_klass, "push_front", "unshift");
  rb_define_method(rb_klass, "shift", RUBY_METHOD_FUNC(rb_shift), 0);
  rb_define_alias(rb_klass, "pop_front", "shift");
  rb_define_method(rb_klass, "each", RUBY_METHOD_FUNC(rb_each), 0);
  rb_define_method(rb_klass, "each_index", RUBY_METHOD_FUNC(rb_each_index), 0);
  rb_define_method(rb_klass, "insert", RUBY_METHOD_FUNC(rb_insert), 2);
  rb_define_method(rb_klass, "remove", RUBY_METHOD_FUNC(rb_remove), 1);
  rb_define_alias(rb_klass, "delete_at", "remove");
  rb_define_method(rb_klass, "clear", RUBY_METHOD_FUNC(rb_clear), 0);
}

VALUE
rb_allocate(VALUE klass)
{
  CvSeq *ptr = ALLOC(CvSeq);
  return Data_Wrap_Struct(klass, 0, cvseq_free, ptr);
}

void
cvseq_free(void *ptr)
{
  if (ptr) {
    CvSeq *seq = (CvSeq*)ptr;
    st_delete(seqblock_klass_table, (st_data_t*)&ptr, NULL);
    if (seq->storage)
      cvReleaseMemStorage(&(seq->storage));
  }
}

/*
 * call-seq:
 *   CvSeq.new(<i>type[,storage]</i>)
 *
 * Return a new CvSeq. <i>type</i> should be following classes.
 *
 * * CvIndex
 * * CvPoint
 */
VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE klass, storage_value;
  CvMemStorage *storage;

  if (rb_scan_args(argc, argv, "11", &klass, &storage_value) > 1) {
    storage_value = CHECK_CVMEMSTORAGE(storage_value);
    storage = CVMEMSTORAGE(storage_value);
  }
  else
    storage = cvCreateMemStorage(0);
  
  if(!rb_obj_is_kind_of(klass, rb_cClass))
    rb_raise(rb_eTypeError, "argument 1 (sequence-block class) should be %s.", rb_class2name(rb_cClass));

  int type = 0, size = 0;
  if (klass == cCvIndex::rb_class()) {
    type = CV_SEQ_ELTYPE_INDEX;
    size = sizeof(CvIndex);
  }
  else if (klass == cCvPoint::rb_class()) {
    type = CV_SEQ_ELTYPE_POINT;
    size = sizeof(CvPoint);
  }
  else if (klass == cCvPoint2D32f::rb_class()) {
    type = CV_SEQ_ELTYPE_POINT;
    size = sizeof(CvPoint2D32f);
  }
  else if (klass == cCvPoint3D32f::rb_class()) {
    type = CV_SEQ_ELTYPE_POINT3D;
    size = sizeof(CvPoint3D32f);
  }
  // todo: more various class will be support.
  if (!size)
    rb_raise(rb_eTypeError, "unsupport %s class for sequence-block.", rb_class2name(klass));
  
  CvSeq* seq = cvCreateSeq(type, sizeof(CvSeq), size, storage);
  DATA_PTR(self) = seq;
  resist_class_information_of_sequence(seq, klass);
  
  return self;
}
    
/*
 * call-seq:
 *   total -> int
 *
 * Return total number of sequence-block.
 */
VALUE
rb_total(VALUE self)
{
  return INT2FIX(CVSEQ(self)->total);
}

/*
 * call-seq:
 *   empty? -> true or false.
 *
 * Return <tt>true</tt> if contain no object, otherwize return <tt>false</tt>.
 */
VALUE
rb_empty_q(VALUE self)
{
  return CVSEQ(self)->total == 0 ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   [<i>index</i>] -> obj or nil
 *
 * Return sequence-block at <i>index</i>.
 */
VALUE
rb_aref(VALUE self, VALUE index)
{
  CvSeq *seq = CVSEQ(self);
  int idx = NUM2INT(index);
  if(!(seq->total > 0))
    return Qnil;
  if (idx >= seq->total)
    rb_raise(rb_eIndexError, "index %d out of sequence", idx);
  return REFER_OBJECT(seqblock_class(seq), cvGetSeqElem(seq, idx), self);
}

/*
 * call-seq:
 *   first -> obj or nil
 *
 * Return first sequence-block.
 */
VALUE
rb_first(VALUE self)
{
  CvSeq *seq = CVSEQ(self);
  if(!(seq->total > 0))
    return Qnil;
  return REFER_OBJECT(seqblock_class(seq), cvGetSeqElem(seq, 0), self);
}

/*
 * call-seq:
 *   last -> obj or nil
 *
 * Return last sequence-block.
 */
VALUE
rb_last(VALUE self)
{
  CvSeq *seq = CVSEQ(self);
  if(!(seq->total > 0))
    return Qnil;
  return REFER_OBJECT(seqblock_class(seq), cvGetSeqElem(seq, -1), self);
}
    
/*
 * call-seq:
 *   h_prev -> seq or nil
 *
 * Return the sequence horizontally located in previous.
 * Return <tt>nil</tt> if not existing.
 */
VALUE
rb_h_prev(VALUE self)
{
  CvSeq *seq = CVSEQ(self);
  if (seq->h_prev)
    return new_sequence(CLASS_OF(self), seq->h_prev, seqblock_class(seq), lookup_root_object(seq));
  else
    return Qnil;
}

/*
 * call-seq:
 *   h_next -> seq or nil
 *
 * Return the sequence horizontally located in next.
 * Return <tt>nil</tt> if not existing.
 */
VALUE
rb_h_next(VALUE self)
{
  CvSeq *seq = CVSEQ(self);
  if (seq->h_next)
    return new_sequence(CLASS_OF(self), seq->h_next, seqblock_class(seq), lookup_root_object(seq));
  else
    return Qnil;
}

/*
 * call-seq:
 *   v_prev -> seq or nil
 *
 * Return the sequence vertically located in previous.
 * Return <tt>nil</tt> if not existing.
 */
VALUE
rb_v_prev(VALUE self)
{
  CvSeq *seq = CVSEQ(self);
  if (seq->v_prev)
    return new_sequence(CLASS_OF(self), seq->v_prev, seqblock_class(seq), lookup_root_object(seq));
  else
    return Qnil;
}

/*
 * call-seq:
 *   v_next -> seq or nil
 *
 * Return the sequence vertically located in next.
 * Return <tt>nil</tt> if not existing.
 */
VALUE
rb_v_next(VALUE self)
{
  CvSeq *seq = CVSEQ(self);
  if (seq->v_next)
    return new_sequence(CLASS_OF(self), seq->v_next, seqblock_class(seq), lookup_root_object(seq));
  else
    return Qnil;
}

VALUE
rb_seq_push(VALUE self, VALUE args, int flag)
{
  CvSeq *seq = CVSEQ(self);
  VALUE klass = seqblock_class(seq), object;
  void *buffer = NULL;
  for (int i = 0; i < RARRAY_LEN(args); i++) {
    object = RARRAY_PTR(args)[i];
    if (CLASS_OF(object) == klass) {
      if (flag == CV_FRONT)
	cvSeqPushFront(seq, DATA_PTR(object));
      else
	cvSeqPush(seq, DATA_PTR(object));
    }
    else if (rb_obj_is_kind_of(object, rb_klass) && CLASS_OF(rb_first(object)) == klass) { // object is CvSeq
      buffer = cvCvtSeqToArray(CVSEQ(object), cvAlloc(CVSEQ(object)->total * CVSEQ(object)->elem_size));
      cvSeqPushMulti(seq, buffer, CVSEQ(object)->total, flag);
      cvFree(&buffer);
    }
    else {
      rb_raise(rb_eTypeError, "arguments should be %s or %s which includes %s.",
	       rb_class2name(klass), rb_class2name(rb_klass), rb_class2name(klass));
    }
  }
  return self;
}

/*
 * call-seq:
 *   push(<i>obj, ...</i>) -> self
 *     
 * Append - Pushes the given object(s) on the end of this sequence. This expression return the sequence itself,
 * so several append may be chained together.
 */
VALUE
rb_push(VALUE self, VALUE args)
{
  return rb_seq_push(self, args, CV_BACK);
}
    
/*
 * call-seq:
 *   pop -> obj or nil
 *
 * Remove the last sequence-block from <i>self</i> and return it,
 * or <tt>nil</tt> if the sequence is empty.
 */
VALUE
rb_pop(VALUE self)
{
  CvSeq *seq = CVSEQ(self);
  if(!(seq->total > 0)){
    return Qnil;
  }
  VALUE object = GENERIC_OBJECT(seqblock_class(seq), malloc(seq->elem_size));
  cvSeqPop(seq, DATA_PTR(object));
  return object;
}

/*
 * call-seq:
 *   clear -> self
 *
 * Clears sequence. Removes all elements from the sequence.
 */
VALUE
rb_clear(VALUE self)
{
  cvClearSeq(CVSEQ(self));
  return self;
}

/*
 * call-seq:
 *   unshift -> self
 *
 * Prepends objects to the front of sequence. other elements up one.
 */
VALUE
rb_unshift(VALUE self, VALUE args)
{
  return rb_seq_push(self, args, CV_FRONT);
}

/*
 * call-seq:
 *   shift -> obj or nil
 *
 * Returns the first element of <i>self</i> and removes it (shifting all other elements down by one). Returns <tt>nil</tt> if the array is empty.
 */
VALUE
rb_shift(VALUE self)
{
  CvSeq *seq = CVSEQ(self);
  if(!(seq->total > 0)){
    return Qnil;
  }
  VALUE object = GENERIC_OBJECT(seqblock_class(seq), malloc(seq->elem_size));
  cvSeqPopFront(seq, DATA_PTR(object));
  return object;
}

/*
 * call-seq:
 *   each{|obj| ... } -> self
 *
 * Calls block once for each sequence-block in <i>self</i>,
 * passing that sequence-block as a parameter.
 *   seq = CvSeq.new(CvIndex)
 *   seq.push(5, 6, 7)
 *   seq.each{|x| print x, " -- " }
 * produces:
 *   5 -- 6 -- 7 --
 */
VALUE
rb_each(VALUE self)
{
  CvSeq *seq = CVSEQ(self);
  if(seq->total > 0){
    VALUE klass = seqblock_class(seq);
    for(int i = 0; i < seq->total; i++){
      rb_yield(REFER_OBJECT(klass, cvGetSeqElem(seq, i), self));
    }
  }
  return self;
}

/*
 * call-seq:
 *    each_index{|index| ... } -> self
 *
 * Same as CvSeq#each, but passes the index of the element instead of the element itself.
 */
VALUE
rb_each_index(VALUE self)
{
  CvSeq *seq = CVSEQ(self);
  for(int i = 0; i < seq->total; i++)
    rb_yield(INT2FIX(i));
  return self;
}


/*
 * call-seq:
 *   insert(<i>index,obj</i>) -> self
 *
 * Inserts the given values before element with the given index (which may be negative).
 */
VALUE
rb_insert(VALUE self, VALUE index, VALUE object)
{
  Check_Type(index, T_FIXNUM);
  CvSeq *seq = CVSEQ(self);
  VALUE klass = seqblock_class(seq);
  if(CLASS_OF(object) != klass)
    rb_raise(rb_eTypeError, "arguments should be %s.", rb_class2name(klass));
  cvSeqInsert(seq, FIX2INT(index), DATA_PTR(object));
  return self;
}

/*
 * call-seq:
 *   remove(<i>index</i>) -> obj or nil
 *
 * Deletes the elements at the specified index.
 */
VALUE
rb_remove(VALUE self, VALUE index)
{
  cvSeqRemove(CVSEQ(self), FIX2INT(index));
  return self;
}

/*
VALUE
new_object(CvSeq *seq, VALUE klass)
{
  VALUE storage = cCvMemStorage::new_object();
  VALUE object = REFER_OBJECT(rb_klass, seq, storage);
  st_insert(seqblock_klass, (st_data_t)seq, (st_data_t)klass);
  return object;
}

VALUE
new_object(CvSeq *seq, VALUE klass, VALUE storage)
{
  VALUE object = REFER_OBJECT(rb_klass, seq, storage);
  st_insert(seqblock_klass, (st_data_t)seq, (st_data_t)klass);
  return object;
}
*/

void
unresist_cvseq(void *ptr)
{
  if (ptr)
    st_delete(seqblock_klass_table, (st_data_t*)&ptr, NULL);
}

VALUE
new_sequence(VALUE klass, CvSeq *seq, VALUE element_klass, VALUE storage)
{
  resist_root_object(seq, storage);
  if (!NIL_P(element_klass))
    resist_class_information_of_sequence(seq, element_klass);
  return Data_Wrap_Struct(klass, mark_root_object, unresist_cvseq, seq);
}

VALUE
auto_extend(VALUE object)
{
  CvSeq *seq = CVSEQ(object);
  if(CV_IS_SEQ_POINT_SET(seq)){
    rb_extend_object(object, mPointSet::rb_module());
  }
  return object;
}

__NAMESPACE_END_CVSEQ
__NAMESPACE_END_OPENCV
