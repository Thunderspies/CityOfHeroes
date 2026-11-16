#########################################################
## Dark Nova Detonation (Hit)
#########################################################

FxInfo

LifeSpan	200

## SOUND FX #######################################################

#Condition
#	On 	Time
#	Time 	0
#	
#	Event
#		EName	SwordFlash2
#		Type	Local
#		At	Chest
#		Sound	WarShadeAORHit 110 70 .93
#	End
#End

#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName	SwordFlash2
		Type	Local
		At	Chest
		Part1	:BlastFlash.part
		Part	:BlastFlashTendrils.part
		Part	:QuickBlastHitStreaks.part
		Part	:PeaceBlastHitSparks.part
		Part	:QuickBlastHitStreaksPurple.part
		LifeSpan 10
	End
End

#########################################################

End		