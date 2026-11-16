#############################################################
## AntimatterPulse.fx
#############################################################

FxInfo
Lifespan 210

Flags	DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	Torment_01 100 100 .8
	End
	Event
		Type	Local
		At	Root
		part	:FlareUp_02.part
		part	:FlareUp_01.part

		Lifespan 30
	End

	Event
		Type	Local
		At	Root
		bhvrOverride
			PositionOffset 0 1.3 0
		End
		part	:FlareUp_Base.part
		#part	:FlareUp_Base02.part

		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	Root
		bhvrOverride
			PositionOffset 0 1.3 0
		End
		part	:Shockwave_Rev.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Type	Local
		At	Root
		bhvrOverride
			PositionOffset 0 1.3 0
		End
		#part	:Glow_Burst.part
		#part	:Glow_Burst_base.part
		part	:Shockwave.part
		part	:Shockwave_base.part
		Lifespan 30
	End
End

#########################	SCREEN EFFECT

Condition
	On	Time
	Time	50

	Event
		Type	Start
		CAMERASPACE
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Clouds_Border.part
		Lifespan 150
	End
End

Condition
	On	Time
	Time	52

	Event
		Type	Start
		CAMERASPACE
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Filter_Purple_Subtractive.part
		Part	:Filter_Purple.part
		Lifespan 150
	End
End

#############################################################

End
