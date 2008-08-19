/************************************************************

  cvhaarclassifercascade.cpp -

  $Author: lsxi $

  Copyright (C) 2005-2007 Masakazu Yonekura

************************************************************/
#include "cvhaarclassifiercascade.h"
/*
 * Document-class: OpenCV::CvHaarClassifierCascade
 *
 * CvHaarClassifierCascade object is "fast-object-detector".
 * This detector can discover object (e.g. human's face) from image.
 *
 * Find face-area from picture "lena"...
 * link:../images/face_detect_from_lena.jpg
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVHAARCLASSIFERCASCADE

VALUE rb_klass;

VALUE
rb_class()
{
  return rb_klass;
}

void define_ruby_class()
{
  if (rb_klass)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   * 
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  
  rb_klass = rb_define_class_under(opencv, "CvHaarClassifierCascade", rb_cObject);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_singleton_method(rb_klass, "load", RUBY_METHOD_FUNC(rb_load), 1);
  rb_define_method(rb_klass, "detect_objects", RUBY_METHOD_FUNC(rb_detect_objects), -1);
  rb_define_method(rb_klass, "detect_objects_with_pruning", RUBY_METHOD_FUNC(rb_detect_objects_with_pruning), -1);
}

VALUE
rb_allocate(VALUE klass)
{
  return OPENCV_OBJECT(klass, 0);
}

/*
 * call-seq:
 *   CvHaarClassiferCascade.load(<i>path</i>) -> object-detector
 * 
 * Load trained cascade of haar classifers from file.
 * Object detection classifiers are stored in XML or YAML files.
 * sample of object detection classifier files is included by OpenCV.
 *
 * You can found these at
 *    C:\Program Files\OpenCV\data\haarcascades\*.xml (Windows, default install path)
 *
 * e.g. you want to try to detect human's face.
 *    detector = CvHaarClassiferCascade.load("haarcascade_frontalface_alt.xml")
 */
VALUE
rb_load(VALUE klass, VALUE path)
{
  CvHaarClassifierCascade *cascade = (CvHaarClassifierCascade*)cvLoad(StringValueCStr(path), 0, 0, 0);
  if(!CV_IS_HAAR_CLASSIFIER(cascade))
    rb_raise(rb_eTypeError, "invalid format haar classifier cascade file.");
  return OPENCV_OBJECT(rb_klass, cascade);
}

VALUE
rb_save(VALUE self, VALUE path)
{
  rb_raise(rb_eNotImpError, "");
}

/*
 * call-seq:
 *   detect_objects(image[,scale_factor = 1.1, min_neighbor = 3, min_size = CvSize.new(0,0)]) -> cvseq(include CvAvgComp object)
 *   detect_objects(image[,scale_factor = 1.1, min_neighbor = 3, min_size = CvSize.new(0,0)]){|cmp| ... } -> cvseq(include CvAvgComp object)
 *
 * Detects objects in the image. This method finds rectangular regions in the
 * given image that are likely to contain objects the cascade has been trained
 * for and return those regions as a sequence of rectangles.
 *
 * * scale_factor (should be > 1.0)
 *     The factor by which the search window is scaled between the subsequent scans, for example, 1.1 mean increasing window by 10%.
 * * min_neighbors
 *     Minimum number (minus 1) of neighbor rectangles that makes up an object.
 *     All the groups of a smaller number of rectangles than min_neighbors - 1 are rejected.
 *     If min_neighbors is 0, the function does not any grouping at all and returns all the detected
 *     candidate rectangles, whitch many be useful if the user wants to apply a customized grouping procedure.
 * * min_size
 *     Minimum window size. By default, it is set to size of samples the classifier has been trained on.
 */
VALUE 
rb_detect_objects(int argc, VALUE *argv, VALUE self)
{ 
  VALUE image, storage, scale_factor, min_neighbors, min_size, result;
  rb_scan_args(argc, argv, "14", &image, &storage, &scale_factor, &min_neighbors, &min_size);
  if (!rb_obj_is_kind_of(image, cCvMat::rb_class()))
    rb_raise(rb_eTypeError, "argument 1(target-image) should be %s.", rb_class2name(cCvMat::rb_class()));
  double scale = IF_DBL(scale_factor, 1.1);
  if (!(scale > 1.0))
    rb_raise(rb_eArgError, "argument 2 (scale factor) must > 1.0.");
  storage = CHECK_CVMEMSTORAGE(storage);
  CvSeq *seq = cvHaarDetectObjects(CVMAT(image), CVHAARCLASSIFIERCASCADE(self), CVMEMSTORAGE(storage),
                                   scale, IF_INT(min_neighbors, 3), 0, NIL_P(min_size) ? cvSize(0,0) : VALUE_TO_CVSIZE(min_size));
  result = cCvSeq::new_sequence(cCvSeq::rb_class(), seq, cCvAvgComp::rb_class(), storage);
  if (rb_block_given_p()) {
    for(int i = 0; i < seq->total; i++)
      rb_yield(REFER_OBJECT(cCvAvgComp::rb_class(), cvGetSeqElem(seq, i), storage));      
  }
  return result;
}

/*
 * call-seq:
 *   detect_objects_with_pruning(image[,scale_factor = 1.1, min_neighbor = 3, min_size = CvSize.new(0,0)]) -> cvseq(include CvAvgComp object)
 *   detect_objects_with_pruning(image[,scale_factor = 1.1, min_neighbor = 3, min_size = CvSize.new(0,0)]){|cmp| ... } -> cvseq(include CvAvgComp object)
 *
 * Almost same to #detect_objects (Return detected objects).
 * 
 * Before scanning to image, Canny edge detector to reject some image regions
 * that contain too few or too much edges, and thus can not contain the searched object.
 *
 *   note: The particular threshold values are tuned for face detection.
 *         And in this case the pruning speeds up the processing.
 */ 
VALUE
rb_detect_objects_with_pruning(int argc, VALUE *argv, VALUE self)
{
  VALUE image, storage, scale_factor, min_neighbors, min_size, result;
  rb_scan_args(argc, argv, "14", &image, &storage, &scale_factor, &min_neighbors, &min_size);
  if (!rb_obj_is_kind_of(image, cCvMat::rb_class()))
    rb_raise(rb_eTypeError, "argument 1(target-image) should be %s.", rb_class2name(cCvMat::rb_class()));
  double scale = IF_DBL(scale_factor, 1.1);
  if (!(scale > 1.0))
    rb_raise(rb_eArgError, "argument 2 (scale factor) must > 1.0.");
  storage = CHECK_CVMEMSTORAGE(storage);
  CvSeq *seq = cvHaarDetectObjects(CVMAT(image), CVHAARCLASSIFIERCASCADE(self), CVMEMSTORAGE(storage),
                                   scale, IF_INT(min_neighbors, 3), CV_HAAR_DO_CANNY_PRUNING, NIL_P(min_size) ? cvSize(0,0) : VALUE_TO_CVSIZE(min_size));
  result = cCvSeq::new_sequence(cCvSeq::rb_class(), seq, cCvAvgComp::rb_class(), storage);
  if (rb_block_given_p()) {
    for(int i = 0; i < seq->total; i++)
      rb_yield(REFER_OBJECT(cCvAvgComp::rb_class(), cvGetSeqElem(seq, i), storage));      
  }
  return result;
}

__NAMESPACE_END_CVHAARCLASSIFERCASCADE
__NAMESPACE_END_OPENCV
