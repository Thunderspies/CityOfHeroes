#############################################################
## DeathAndGlory_Attack.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local
		At	Root
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		Part	:HealAOE_Ring_Burst.part
		Part	:HealAOE_Rings.part
		Lifespan	60
	End
End

#############################################################

End