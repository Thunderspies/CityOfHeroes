#############################################################
## DarknessAbsorbPainWithRays2.fx
#############################################################

FxInfo
Lifespan	50

########################################################

Input
	Inpname	Hips
End

########################################################



Condition
	On 	Time
	Time 	5

	Event
		ENAME   Tendrills
		Type	Local
		At	chest
		Part3	CustomizeablePowers\DarkMelee\DamageHitMist2.part
		Part1	CustomizeablePowers\DarkMelee\SoulDrainHitTendril2.part
		Part2	CustomizeablePowers\DarkMelee\SoulDrainHitPuddle2.part
		Part4	CustomizeablePowers\DarkMelee\RedRays.part
		LifeSpan	40
	End

End

End
##################################

