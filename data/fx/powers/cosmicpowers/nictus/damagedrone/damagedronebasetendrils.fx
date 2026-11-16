#########################################################
##	Inferno FX
##
FxInfo
########################################################

Condition
	On 	cycle
	Time 	2
	Chance	.7

	Event
		Type	start 
		At	Root
		
		Bhvr	:BlueTendrilsIn.bhvr
		Splat	Tendril01.tga

		Pmagnet	Chest

		LifeSpan	10
	End

End


Condition
	On 	cycle
	Time 	2
	Chance	.7

	Event
		Type	start 
		At	Root
		
		Bhvr	:PurpleTendrilsIn.bhvr
		Splat	Tendril01.tga

		Pmagnet	Chest
		
		LifeSpan	10
	End

End


Condition
	On 	cycle
	Time 	2
	Chance	.7

	Event
		Type	start 
		At	Root
		
		Bhvr	:BlackTendrilsIn.bhvr
		Splat	Tendril01.tga

		Pmagnet	Chest
		
		LifeSpan	10
	End

End

End
