#########################################################
##	template

FxInfo

LifeSpan	80
#########################################################
####################### AUDIO ###########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound RobotRepair 80 80 .87
	End
End


#########################################################
#################### FX ########################
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		Type	local
		At	Root

		Part	:Electricity.part
		Part	:Electricity2.part
		Part	:ElectricitySparkUp.part
		Part	:ElectricitySparkUp2.part
		
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	Head
		ChildFx		World/Ruins/StreetSparks.fx
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
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
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	10
	End

End

######################################################################################################

Condition

	Event
		Type	Local 
		At	Head
		
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50
		
	End

	Event
		Type	Local 
		At	neck
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50
				
	End

	Event
		Type	Local 
		At	CHEST
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50
	End

	Event
		Type	Local 
		At	Hips
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50
	End

	Event
		Type	Local 
		At	UARML
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50

	End

	Event
		Type	Local 
		At	UARMR
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50

	End

	Event
		Type	Local 
		At	LARMR
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50

	End

	Event
		Type	Local 
		At	LARML
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50
	End

	Event
		Type	Local 
		At	WepL
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50
	End

	Event
		Type	Local 
		At	WepR
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50

	End

	Event
		Type	Local 
		At	UlegR
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50

	End

	Event
		Type	Local 
		At	ULEGL
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50

	End

	Event
		Type	Local 
		At	LLEGL
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50
	End

	Event
		Type	Local 
		At	LLEGR
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50
	End

	Event
		Type	Local 
		At	FOOTL
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50
		
	End

	Event
		Type	Local 
		At	FOOTR
		
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/HealEnergy.part
		Part		V_COV/Powers/Heal/Heal.part
		LifeSpan	50
	End

End

End		