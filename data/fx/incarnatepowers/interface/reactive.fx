#############################################################
## Lingering Fire
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	Chest
		part	:Flash.part
		Lifespan 5
	End

End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Reactive_Loop 100 100 .3
		bhvr	behaviors/Sound/SoundIn05Out10.bhvr
		lifespan 150
	End

	Event
		Type	Local
		At	Chest
		part	:Glow2.part
		part2	:GoldenCenter.part
		part3	:Lingering_Cloud.part
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core.part
		Part	:Plasma_Highlight.part
		BhvrOverride
			PositionOffset	0.0 -0.3 0
		End
		POther	Neck
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.3 0
		End
		Part	:Plasma_Core.part
		Part	:Plasma_Highlight.part
		POther	Neck
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0 0.4 0
		End
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Plasma_Core.part
		Part	:Plasma_Highlight.part
	End

	Event
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0.7 0
		End
		Part	:Plasma_CoreA.part
	End

End

#############################################################

End