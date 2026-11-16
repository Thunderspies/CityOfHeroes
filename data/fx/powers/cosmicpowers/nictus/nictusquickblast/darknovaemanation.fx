#########################################################
## Dark Nova - Emenation (Attack FX)
#########################################################

FxInfo

LifeSpan 200

## SOUND FX ######################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Start
		At	Head
		Sound WarshadeQuick 80 80 .93
	End
End


## ATTACK FX ######################################################

Condition
	On 	Time
	Time 	28

	Event
		EName	SwordFlash2
		Type	Local
		At	head
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandStreaksBlue.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		LifeSpan 5
	End
End

#########################################################

End