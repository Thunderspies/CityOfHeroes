#############################################################
## DarknessAbsorbPainWithRays2_Subtractive.fx
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
		Part1	Powers/DarknessControl/SoulDrainHitTendril2_Subtractive.part
		Part	Powers/DarknessControl/SoulDrainHitPuddle2_Subtractive.Part
		Part3	Powers/DarknessControl/DamageHitMist2.part
		Part1	Powers/DarknessControl/SoulDrainHitTendril2.part
		Part2	Powers/DarknessControl/SoulDrainHitPuddle2.part
		Part4	Powers/DarknessControl/RedRays.part
		LifeSpan	40
	End

End

End
##################################

