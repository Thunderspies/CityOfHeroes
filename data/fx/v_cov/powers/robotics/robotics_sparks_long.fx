#########################################################
##	template

FxInfo

LifeSpan	300
#########################################################
####################### AUDIO ###########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		#Sound	Glow3_loop 80 30 .26
	End
End


#########################################################
#################### FX ########################
#########################################################

#Condition
#	On	Time
#	Time	0
#	
#	Event	
#		Type	local
#		At	Root
#
#		Part	:Electricity.part
#		Part	:Electricity2.part
#		Part	:ElectricitySparkUp.part
#		Part	:ElectricitySparkUp2.part
#		
#	End
#
#End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	Head
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10
		
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	neck
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10
				
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	CHEST
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	Hips
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10
	End


End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	UARML
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	UARMR
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	LARMR
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	LARML
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	WepL
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	WepR
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	UlegR
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	ULEGL
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	LLEGL
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	LLEGR
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	FOOTL
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10
		
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	FOOTR
		
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10
	End

End

End		