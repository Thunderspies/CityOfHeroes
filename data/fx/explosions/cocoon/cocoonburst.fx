#########################################################
##	onfire
##
FxInfo  

LifeSpan	110

Condition
	On 	Time
	Time 	0

	Event
		Type	start 
		At	chest
		Sound cocoonexplo1 88 88 .9
		ChildFx	:SmutchBurst.fx
		
	End

	Event
		Type	start 
		At	chest
		

		Part	:Webstrands.part
		LifeSpan	10
	End

	Event
		Type	start 
		At	chest

		Part	:MistBright.part
		Part	:Mist.part
		Part	:WebDust.part
	End
End


Condition
	On 	Time
	Time 	5

	Event
		Type	start 
		At	chest
		

		Part	:Webstrands.part
		LifeSpan	10
	End

End

End			