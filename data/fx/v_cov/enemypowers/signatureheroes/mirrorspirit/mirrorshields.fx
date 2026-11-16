#########################################################
##	Mirror Shields
########################################################
FxInfo

Flags	IsArmor	InheritAlpha

############################################


Condition
	On	Time	
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound Magicshield3_loop 69.0 69.0 .44
		#bhvr	behaviors/soundFade3.bhvr
		#Flags	PowerLoopingSound
		#Lifespan 420
	End

End


Condition
	On	Time	
	Time 	25

	Event
		Type 	Local
		At	Origin
		Sound Magicshield_loop 69.0 69.0 .69
		#bhvr	behaviors/soundFade3.bhvr
		#Flags	PowerLoopingSound
		#Lifespan 420
	End

End



############################################

Condition
	On	Time
	Time	25

	Event
		Ename	SwirlForce
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset		0 2 0

			PhysForceType		CWSwirl
			PhysForceRadius		12
			PhysForcePower		9
			PhysForcePowerJitter	3
			
		End

	End

		Event
		Ename	LiftForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		4
			PhysForcePower		30
			PhysForcePowerJitter	5
			
		End

	End

End

Condition
	On 	Time
	Time 	0

	Event
		Ename	Hands01
		Type	Local
		At	WepR

		bhvr	behaviors/GenericParticleFade.bhvr

		Part	:MirrorSmokeTrail.part	
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part
		Part	:MirrorSparkGlow.part
		Sound Magic_shieldup 69.0 69.0 .66
		Lifespan	45
	End

	Event
		Ename	Hands02
		Type	Local
		At	WepL
		
		bhvr	behaviors/GenericParticleFade.bhvr

		Part	:MirrorSmokeTrail.part	
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part
		Part	:MirrorSparkGlow.part
		Lifespan	45
	End

End


###############  Mirror 01  #####################

Condition
	On 	Time
	Time 	20

	Event
		Ename	Spin01
		Type	Local
		At	Root
		bhvr	:MirrorSpinOut.bhvr
		BhvrOverride
			PositionOffset	0 4 0
			Spin		0 0.05 0
			#pyrRotateJitter	.2 0 .2
		End
				
	End

	Event
		Ename	Offset01
		Type	Local
		At	Spin01
		BhvrOverride
			PositionOffset	0 0 4
		End
				
	End

	Event
		Ename	Case01
		Type	Local
		At	Offset01
		bhvr	:MirrorFade.bhvr
		BhvrOverride
			Spin		0 0.1 0
			SpinJitter	0.05 0.02 0.05
			pyrRotate	180 0 0 #-1.5708 0 0
		End
		Geom	FX_BronzeMirror_Back		
	End

	Event
		Ename	Mirror01
		Type	Local
		At	Case01
		bhvr	:MirrorFade.bhvr
		
		Geom	FX_BronzeMirror	
		
		Part	:MirrorSmokeTrail.part	
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part
	End

###############  Mirror 02  #####################

	Event
		Ename	Spin02
		Type	Local
		At	Root
		bhvr	:MirrorSpinOut.bhvr
		BhvrOverride
			PositionOffset	0 4 0
			Spin		0 0.05 0
			pyrRotate	0 180 0
			#pyrRotateJitter	.2 0 .2 
		End
				
	End

	Event
		Ename	Offset02
		Type	Local
		At	Spin02
		BhvrOverride
			PositionOffset	0 0 4
		End
				
	End

	Event
		Ename	Case02
		Type	Local
		At	Offset02
		
		bhvr	:MirrorFade.bhvr
		BhvrOverride
			Spin		0 0.1 0
			SpinJitter	0.05 0.02 0.05
			pyrRotate	180 0 0 #-1.5708 0 0
		End
		Geom	FX_BronzeMirror_Back		
	End

	Event
		Ename	Mirror02
		Type	Local
		At	Case02
		
		bhvr	:MirrorFade.bhvr
		Geom	FX_BronzeMirror	

		Part	:MirrorSmokeTrail.part		
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part
	End

###############  Mirror 03  #####################

	Event
		Ename	Spin03
		Type	Local
		At	Root
		bhvr	:MirrorSpinOut.bhvr
		BhvrOverride
			PositionOffset	0 4 0
			pyrRotate	0 90 0
			Spin		0 0.05 0
			#pyrRotateJitter	.2 0 .2
		End
				
	End

	Event
		Ename	Offset03
		Type	Local
		At	Spin03
		BhvrOverride
			PositionOffset	0 0 4
		End
				
	End

	Event
		Ename	Case03
		Type	Local
		At	Offset03
		
		bhvr	:MirrorFade.bhvr
		BhvrOverride
			Spin		0 0.1 0
			SpinJitter	0.05 0.02 0.05
			pyrRotate	180 0 0 #-1.5708 0 0
		End
		Geom	FX_BronzeMirror_Back		
	End

	Event
		Ename	Mirror03
		Type	Local
		At	Case03
		
		bhvr	:MirrorFade.bhvr
		Geom	FX_BronzeMirror	

		Part	:MirrorSmokeTrail.part		
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part	
	End

###############  Mirror 04  #####################

	Event
		Ename	Spin04
		Type	Local
		At	Root
		bhvr	:MirrorSpinOut.bhvr
		BhvrOverride
			PositionOffset	0 4 0
			pyrRotate	0 -90 0
			Spin		0 0.05 0
			#pyrRotateJitter	.2 0 .2
		End
				
	End

	Event
		Ename	Offset04
		Type	Local
		At	Spin04
		BhvrOverride
			PositionOffset	0 0 4
		End
				
	End

	Event
		Ename	Case04
		Type	Local
		At	Offset04
		
		bhvr	:MirrorFade.bhvr
		BhvrOverride
			Spin		0 0.1 0
			SpinJitter	0.05 0.02 0.05
			pyrRotate	180 0 0 #-1.5708 0 0
		End
		Geom	FX_BronzeMirror_Back		
	End

	Event
		Ename	Mirror04
		Type	Local
		At	Case04
		
		bhvr	:MirrorFade.bhvr
		Geom	FX_BronzeMirror	

		Part	:MirrorSmokeTrail.part		
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part	
	End

End



Condition
	On 	Time
	Time 	35

###############  Mirror 05  #####################

	Event
		Ename	Spin05
		Type	Local
		At	Root
		bhvr	:MirrorSpinOut.bhvr
		BhvrOverride
			PositionOffset	0 2 0
			Spin		0 -0.05 0
			#pyrRotateJitter	.2 0 .2
		End
				
	End

	Event
		Ename	Offset05
		Type	Local
		At	Spin05
		BhvrOverride
			PositionOffset	0 0 3
		End
				
	End

	Event
		Ename	Case05
		Type	Local
		At	Offset05
		bhvr	:MirrorFade.bhvr
		BhvrOverride
			Spin		0 0.075 0
			SpinJitter	0.025 0.025 0.0255
			pyrRotate	180 0 0 #-1.5708 0 0
		End
		Geom	FX_BronzeMirror_Back		
	End

	Event
		Ename	Mirror05
		Type	Local
		At	Case05
		bhvr	:MirrorFade.bhvr
		
		Geom	FX_BronzeMirror	

		Part	:MirrorSmokeTrail.part		
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part	
	End

###############  Mirror 06  #####################

	Event
		Ename	Spin06
		Type	Local
		At	Root
		bhvr	:MirrorSpinOut.bhvr
		BhvrOverride
			PositionOffset	0 2 0
			Spin		0 -0.05 0
			pyrRotate	0 180 0
			#pyrRotateJitter	.2 0 .2 
		End
				
	End

	Event
		Ename	Offset06
		Type	Local
		At	Spin06
		BhvrOverride
			PositionOffset	0 0 3
		End
				
	End

	Event
		Ename	Case06
		Type	Local
		At	Offset06
		
		bhvr	:MirrorFade.bhvr
		BhvrOverride
			Spin		0 0.075 0
			SpinJitter	0.025 0.025 0.025
			pyrRotate	180 0 0 #-1.5708 0 0
		End
		Geom	FX_BronzeMirror_Back		
	End

	Event
		Ename	Mirror06
		Type	Local
		At	Case06
		
		bhvr	:MirrorFade.bhvr
		Geom	FX_BronzeMirror	

		Part	:MirrorSmokeTrail.part		
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part	
	End

###############  Mirror 07  #####################

	Event
		Ename	Spin07
		Type	Local
		At	Root
		bhvr	:MirrorSpinOut.bhvr
		BhvrOverride
			PositionOffset	0 2 0
			pyrRotate	0 90 0
			Spin		0 -0.05 0
			#pyrRotateJitter	.2 0 .2
		End
				
	End

	Event
		Ename	Offset07
		Type	Local
		At	Spin07
		BhvrOverride
			PositionOffset	0 0 3
		End
				
	End

	Event
		Ename	Case07
		Type	Local
		At	Offset07
		
		bhvr	:MirrorFade.bhvr
		BhvrOverride
			Spin		0 0.075 0
			SpinJitter	0.025 0.025 0.025
			pyrRotate	180 0 0 #-1.5708 0 0
		End
		Geom	FX_BronzeMirror_Back		
	End

	Event
		Ename	Mirror07
		Type	Local
		At	Case07
		
		bhvr	:MirrorFade.bhvr
		Geom	FX_BronzeMirror	

		Part	:MirrorSmokeTrail.part	
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part	
	End

###############  Mirror 08  #####################

	Event
		Ename	Spin08
		Type	Local
		At	Root
		bhvr	:MirrorSpinOut.bhvr
		BhvrOverride
			PositionOffset	0 2 0
			pyrRotate	0 -90 0
			Spin		0 -0.05 0
			#pyrRotateJitter	.2 0 .2
		End
				
	End

	Event
		Ename	Offset08
		Type	Local
		At	Spin08
		BhvrOverride
			PositionOffset	0 0 3
		End
				
	End

	Event
		Ename	Case08
		Type	Local
		At	Offset08
		
		bhvr	:MirrorFade.bhvr
		BhvrOverride
			Spin		0 0.075 0
			SpinJitter	0.025 0.025 0.025
			pyrRotate	180 0 0 #-1.5708 0 0
		End
		Geom	FX_BronzeMirror_Back		
	End

	Event
		Ename	Mirror08
		Type	Local
		At	Case08
		
		bhvr	:MirrorFade.bhvr
		Geom	FX_BronzeMirror	

		Part	:MirrorSmokeTrail.part	
		Part	:MirrorGlow.part
		Part	:MirrorSparkTrail.part	
	End

End


End
