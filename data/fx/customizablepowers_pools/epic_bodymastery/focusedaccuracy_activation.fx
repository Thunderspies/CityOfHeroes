#########################################################
##	Fireweapon
########################################################
FxInfo

Input
	Inpname	Head
End

Lifespan	60
########################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local
		At	Head

		Part1	:Agility.part
		Part2	:AgilitySmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Sound mabuildup2 70 70 .7

	End

End

End
##################################

