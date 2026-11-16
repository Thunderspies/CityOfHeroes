#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo
Lifespan 9

#############################################################
Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_Servo_01 150 150 .5
		Sound WW_Servo_02 150 150 .5
		Sound WW_Servo_03 150 150 .5
		Sound WW_Servo_04 150 150 .5
		Sound WW_Servo_05 150 150 .5
		Sound WW_Servo_06 150 150 .5
		# Sound WW_Servo_07 150 150 .5
	End

End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_FF_01 200 200 .65
		Sound WW_FF_02 200 200 .65
		Sound WW_FF_03 200 200 .65
		Sound WW_FF_04 200 200 .65
		Sound WW_FF_05 200 200 .65
		Sound WW_FF_06 200 200 .65
		Sound WW_FF_07 200 200 .65
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_FFClose_01 75 75 .9
		Sound WW_FFClose_02 75 75 .9
		Sound WW_FFClose_03 75 75 .9
		Sound WW_FFClose_04 75 75 .9
		Sound WW_FFClose_05 75 75 .9
		Sound WW_FFClose_06 75 75 .9
		Sound WW_FFClose_07 75 75 .9
	End

	Event
		Type	StartPositOnly
		At	FootL
		Bhvr	behaviors/CameraShake01Subtle.bhvr
		BhvrOverride
			Shake		0.3
			PositionOffset	0.0 -3.0 0.0
		End
		Part	:Dust_Circle_Footstep.part
		Lifespan 5
	End
End
