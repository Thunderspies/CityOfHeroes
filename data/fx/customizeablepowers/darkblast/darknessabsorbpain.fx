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
		Part3	:DamageHitMist2.part
		Part1	:SoulDrainHitTendril2.part
		Part2	:SoulDrainHitPuddle2.part

		LifeSpan	60
	End

End

End
##################################

