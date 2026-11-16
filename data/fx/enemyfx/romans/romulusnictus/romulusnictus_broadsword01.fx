#########################################################
## Warshade - Eye Glow Continuing FX
########################################################

FxInfo

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SwordTip
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset		0.0 0.0 4
		End
	End

	Event
		Type	Local
		At	Origin
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlueTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_PurpleTendrils2.part
		Part	ENEMYFX\Romans\RomulusNictus\Aura_BlackTendrils2.part
		pOther	SwordTip
	End
End

#############################################################

End