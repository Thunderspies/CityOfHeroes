#############################################################
## CallReinforcements_Attack.fx
#############################################################

FxInfo
LifeSpan	90

#############################################################

Condition 
	Event
		Type Local
		At Head
		Sound PPD_MonitorSphere_CallOpen_01 100 100 .9
	End
End

Condition
	On	Time
	Time	55

	Event
		EName	HeadAnchor
		At	Head
		Type	Local
		BhvrOverride
			PositionOffset	0.0 -0.05 0.45
		End
	End
End

Condition
	On 	Cycle
	Time 	9

	Event
		Type	Local
		At	HeadAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Signal_Light.part
		Part	:Signal_Flash.part
		Part	:Signal_Light_Glow.part
		Lifespan 3
	End
End

Condition
	On 	Cycle
	Time 	18

	Event
		Type	Local
		At	HeadAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Signal_Rings.part
		Lifespan 2
	End

End

#########################################################################################


End


