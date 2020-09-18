;; Auto-generated. Do not edit!


(when (boundp 'motor_control::Floats_array)
  (if (not (find-package "MOTOR_CONTROL"))
    (make-package "MOTOR_CONTROL"))
  (shadow 'Floats_array (find-package "MOTOR_CONTROL")))
(unless (find-package "MOTOR_CONTROL::FLOATS_ARRAY")
  (make-package "MOTOR_CONTROL::FLOATS_ARRAY"))
(unless (find-package "MOTOR_CONTROL::FLOATS_ARRAYREQUEST")
  (make-package "MOTOR_CONTROL::FLOATS_ARRAYREQUEST"))
(unless (find-package "MOTOR_CONTROL::FLOATS_ARRAYRESPONSE")
  (make-package "MOTOR_CONTROL::FLOATS_ARRAYRESPONSE"))

(in-package "ROS")





(defclass motor_control::Floats_arrayRequest
  :super ros::object
  :slots (_req ))

(defmethod motor_control::Floats_arrayRequest
  (:init
   (&key
    ((:req __req) 0)
    )
   (send-super :init)
   (setq _req (round __req))
   self)
  (:req
   (&optional __req)
   (if __req (setq _req __req)) _req)
  (:serialization-length
   ()
   (+
    ;; int8 _req
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int8 _req
       (write-byte _req s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int8 _req
     (setq _req (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> _req 127) (setq _req (- _req 256)))
   ;;
   self)
  )

(defclass motor_control::Floats_arrayResponse
  :super ros::object
  :slots (_res ))

(defmethod motor_control::Floats_arrayResponse
  (:init
   (&key
    ((:res __res) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _res __res)
   self)
  (:res
   (&optional __res)
   (if __res (setq _res __res)) _res)
  (:serialization-length
   ()
   (+
    ;; float32[] _res
    (* 4    (length _res)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32[] _res
     (write-long (length _res) s)
     (dotimes (i (length _res))
       (sys::poke (elt _res i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32[] _res
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _res (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _res i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;;
   self)
  )

(defclass motor_control::Floats_array
  :super ros::object
  :slots ())

(setf (get motor_control::Floats_array :md5sum-) "0224123962c7a33069a097b5e752decf")
(setf (get motor_control::Floats_array :datatype-) "motor_control/Floats_array")
(setf (get motor_control::Floats_array :request) motor_control::Floats_arrayRequest)
(setf (get motor_control::Floats_array :response) motor_control::Floats_arrayResponse)

(defmethod motor_control::Floats_arrayRequest
  (:response () (instance motor_control::Floats_arrayResponse :init)))

(setf (get motor_control::Floats_arrayRequest :md5sum-) "0224123962c7a33069a097b5e752decf")
(setf (get motor_control::Floats_arrayRequest :datatype-) "motor_control/Floats_arrayRequest")
(setf (get motor_control::Floats_arrayRequest :definition-)
      "int8 req
---
float32[] res
")

(setf (get motor_control::Floats_arrayResponse :md5sum-) "0224123962c7a33069a097b5e752decf")
(setf (get motor_control::Floats_arrayResponse :datatype-) "motor_control/Floats_arrayResponse")
(setf (get motor_control::Floats_arrayResponse :definition-)
      "int8 req
---
float32[] res
")



(provide :motor_control/Floats_array "0224123962c7a33069a097b5e752decf")


