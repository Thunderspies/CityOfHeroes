#########################################################
##	template

FxInfo

#Lifespan	44

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound poisongasexplo 80 80 .77
	End
End



Condition
	On 	Time
	Time 	0

	Event
		EName 	arrow
		Type	Local
		At	chest
		Part	:AcidHit.part
		Part	:AcidHitDark.part
		Part	:AcidBubblesHit.part
		Part	:AcidBubblesdarkHit.part
		Part	:AcidBubbleHit.part
		Sound poisongas1_loop 70 70 .28
#		Lifespan	44

	End
	
End


End

		