#############################################################
## AntimatterPulse.fx
#############################################################

FxInfo
Lifespan 210

Flags	DontSuppress

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		Bhvr	behaviors/CameraShake02.bhvr
		ChildFX	:NuclearExplosion_Windup.fx
		Sound Antimatter_Pulse_01 200 200 .65
		Lifespan 5
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Start
		CAMERASPACE
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Filter_Green_Light.part
		Part	:Clouds_Green.part


		Lifespan 210
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	AshAnchor
		Type	Start
		CAMERASPACE
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0.0 10.0 0.0
		End
		Part	:Ashes_Light.part
		Part	:Embers_Light.part
		Lifespan	80
	End
End

#############################################################

End
