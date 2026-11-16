#############################################################
## Child_SteamJet_Small.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Jaw
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.1 0.35
			PyrRotate	-90.0 0.0 -0.0
		End
		Part	:Breath_Jet.part
		Lifespan 	10
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Jaw
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.1 0.35
			PyrRotate	-90.0 0.0 -0.0
		End
		Part	:Breath_Plume.part
		Lifespan 	16
		LifespanJitter	5
	End
End

##############################################################

End