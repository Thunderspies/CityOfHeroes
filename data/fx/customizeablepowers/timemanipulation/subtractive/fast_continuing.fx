#############################################################
## Fast_Continuing.fx
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
		Sound 	TM_Fast_Continuing_Loop 100 100 .1
		bhvr	behaviors/Sound/SoundIn2Out10.bhvr
		lifespan 500
	End
End

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Streaks_Explode_Continuing.part
		Part	:Streaks_Explode_Moving.part
		Part	:Swirl_Explode_Small_Highlights.part
		Part	:Swirl_Explode_Small_Highlights2.part
		Part	:Swirl_Explode_Small.part
		Part	:Spiral_Explode_Small.part
		Lifespan 20
	End
End

#############################################################

End