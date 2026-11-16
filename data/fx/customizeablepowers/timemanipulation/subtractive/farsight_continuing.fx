#############################################################
## Farsight_Continuing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TM_Farsight_Continuing_Loop 100 100 .1
		bhvr	behaviors/Sound/SoundIn2Out10.bhvr
		lifespan 500
	End
End


Condition
	On	Time
	Time	0

	Event

		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Streaks_Detached_Head.part
		Part	:Streaks_Attached_Head.part
		CHILDFX	:CHILD_Farsight_Continuing.fx
		Lifespan	8
	End
End

Condition
	On	Cycle
	Time	60

	Event

		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Streaks_Detached_Head.part
		Part	:Streaks_Attached_Head.part
		CHILDFX	:CHILD_Farsight_Continuing.fx
		Lifespan	8
	End

End

#############################################################

End