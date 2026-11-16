#############################################################
## Peacebringer - Shield Continuing FX
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Posit
		At	HIPS
		Bhvr 	:PeaceShieldBHVR.bhvr
		Geom	CosmicPeaceBringerShield
	End

	Event
		Type	Local
		At	Waist
		Part4	POWERS\CosmicPowers\PeaceBringer\PeaceCoreSparks2.part
	End
End

#############################################################

End