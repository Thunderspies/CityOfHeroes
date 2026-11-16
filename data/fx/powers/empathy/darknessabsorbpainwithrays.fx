#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	chest
End

Lifespan	50

########################################################



Condition
	On 	Time
	Time 	0

	Event
		ENAME   Tendrills
		Type	Local 
		At	chest
		Part3	Powers/DarknessControl/DamageHitMist2.part
		Part1	Powers/DarknessControl/SoulDrainHitTendril2.part
		Part2	Powers/DarknessControl/SoulDrainHitPuddle2.part
		Part4	Powers/DarknessControl/RedRays.part
		SOund	dclifedrain2 80 33 1.0
		LifeSpan	30
	End

End

End
##################################

