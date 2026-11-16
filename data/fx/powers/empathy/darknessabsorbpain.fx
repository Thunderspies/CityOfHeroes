#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	chest
End

LifeSpan	60

########################################################



Condition
	On 	Time
	Time 	5

	Event
		ENAME   Tendrills
		Type	Local 
		At	chest
		Part3	Powers/DarknessControl/DamageHitMist2.part
		Part1	Powers/DarknessControl/SoulDrainHitTendril2.part
		Part2	Powers/DarknessControl/SoulDrainHitPuddle2.part
	
		LifeSpan	60
	End

End

End
##################################

