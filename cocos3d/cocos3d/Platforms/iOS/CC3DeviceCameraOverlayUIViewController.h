/*
 * CC3DeviceCameraOverlayUIViewController.h
 *
 * cocos3d 2.0.0
 * Author: Bill Hollings
 * Copyright (c) 2010-2013 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * http://en.wikipedia.org/wiki/MIT_License
 */

/** @file */	// Doxygen marker

#import "CC3UIViewController.h"

#if CC3_IOS

/**
 * Support for AV capture on Android currently not available through Apportable.
 * When this restriction is lifted, set CC3_AV_CAPTURE_SUPPORTED=1 in the compiler
 * build setting GCC_PREPROCESSOR_DEFINITIONS in your build configuration.
 */
#ifndef CC3_AV_CAPTURE_SUPPORTED
#	define CC3_AV_CAPTURE_SUPPORTED		!(APPORTABLE)
#endif

#if CC3_AV_CAPTURE_SUPPORTED

#import <AVFoundation/AVCaptureVideoPreviewLayer.h>

@class CC3AVCameraView;

#pragma mark -
#pragma mark CC3DeviceCameraOverlayUIViewController

/**
 * CC3AugmentedRealityUIViewController is a CC3UIViewController that adds the ability to
 * display the 3D scene as an overlay on a background generated by the device camera.
 */
@interface CC3DeviceCameraOverlayUIViewController : CC3UIViewController {
	CC3AVCameraView* _deviceCameraView;
	BOOL _isOverlayingDeviceCamera : 1;
}

/** Indicates whether this device supports a camera. */
@property(nonatomic, readonly) BOOL isDeviceCameraAvailable;

/**
 * Controls whether the controlled CCNode is overlaying the view of the device camera.
 *
 * This property can only be set to YES if a camera is actually available on the device.
 *
 * If the device supports a camera, setting this property to YES will cause the controller
 * to immediately open a view of the device camera and overlay the CCNode view on top of
 * the device camera view.
 *
 * Setting this property to NO will cause the controller to close the device camera
 * (if it was open) and display the CCNode without the camera background.
 *
 * This property should only be set once the view has been added to the window and the
 * window has been keyed and made visible.
 *
 * Displaying the scene overlaying the device camera requires combining two graphic scenes.
 * This is not without a performance cost, and you should expect to see a drop in animation
 * frame rate as a result.
 *
 * Converting back and forth between the device camera overlay and a normal view is not a trivial
 * activity. The simple act of changing this property causes the following sequence of actions:
 *
 *   - If the CCNode is currently running, it is sent an onExit message to cause it to
 *     stop running, clean up any active actions, and reset its touch event handling.
 *     CCNode subclasses can also override onExit to perform other activities associated
 *     with cleaning up prior to the overlay changing.
 *
 *   - This controller is sent a willChangeIsOverlayingDeviceCamera message.
 *
 *   - The isOverlayingDeviceCamera property of this controller is changed.
 *
 *   - If this property is being set to YES, the CC3AVCameraView instance in the deviceCameraView
 *     property is added to the window behind the view of this controller. If this property is
 *     being set to NO, the CC3AVCameraView instance is removed from the window.
 *
 *   - This controller is sent a didChangeIsOverlayingDeviceCamera message.
 *
 *   - If the CCNode was running, it is sent an onEnter message to cause it to restart,
 *     be ready for actions, and, in the case of CCLayers, re-register for touch events.
 *     CCNode subclasses can also override onEnter to perform other activities associated
 *     with adjusting their world following the overlay changing (such as hiding or showing
 *     child CCNodes based on whether or not the device camera is now overlayed.
 *
 * The value of this property is initially set to NO.
 */
@property(nonatomic, assign, readwrite) BOOL isOverlayingDeviceCamera;

/**
 * Invoked automatically just before the isOverlayingDeviceCamera property is about to be
 * changed, and before the deviceCameraView has been added to or removed from the window.
 * The isOverlayingDeviceCamera property still has the old value when this call is made.
 * Default does nothing. Subclasses can override
 */
-(void) willChangeIsOverlayingDeviceCamera;

/**
 * Invoked automatically just after the isOverlayingDeviceCamera property has been changed, and after
 * the deviceCameraView has been added to or removed from the window. The isOverlayingDeviceCamera
 * property has the new value when this call is made. Default does nothing. Subclasses can override
 */
-(void) didChangeIsOverlayingDeviceCamera;

/** 
 * Returns the view being used to display the device camera behind the 3D scene, lazily
 * creating if if necessary, or returns nil if the device does not support a camera.
 */
@property(nonatomic, strong, readonly) CC3AVCameraView* deviceCameraView;

@end


#pragma mark -
#pragma mark CC3AVCameraView

/** A UIView that holds a AVCaptureVideoPreviewLayer layer for displaying the device camera feed. */
@interface CC3AVCameraView : UIView

/** The underlying layer cast as a AVCaptureVideoPreviewLayer. */
@property(nonatomic, strong, readonly) AVCaptureVideoPreviewLayer* layer;
@end

#else	// If no AV, fall back to CC3UIViewController
#	define CC3DeviceCameraOverlayUIViewController	CC3UIViewController
#endif	// CC3_AV_CAPTURE_SUPPORTED

#endif // CC3_IOS


