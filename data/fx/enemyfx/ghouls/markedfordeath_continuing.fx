#############################################################
## MarkedForDeath_Continuing.fx
#############################################################

FxInfo
Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local
		At	SpadL
		Part	:Continuing_Fire.part
		sound 	heal4 50 30 .55
	End

	Event
		Type	Local
		At	SpadR
		Part	:Continuing_Fire.part
	End


	Event
		Type	Local
		At	Chest
		Part	:Continuing_Fire.part
		Part	:Continuing_Sparks.part
		Part	:Continuing_Fire_Rings.part
		Part	:Continuing_Trail_Plasma.part
	End
End

#############################################################

End