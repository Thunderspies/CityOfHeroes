#########################################################
##	template

FxInfo

LifeSpan	240
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
#	Time	110
#
#	Event	
#		Ename	initialTiming
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
#
Condition
	On	Time
	Time	115
	
	Event	
		Ename	headhookup
		Type	local
		At	head

		
	End

	Event	
		Ename	LarmRhookup
		Type	local
		At	LarmR

		
	End

	Event	
		Ename	LarmLhookup
		Type	local
		At	LarmL

		
	End
##

	Event	
		Ename	UarmRhookup
		Type	local
		At	UarmR

		
	End

	Event	
		Ename	UarmLhookup
		Type	local
		At	UarmL

		
	End

	Event	
		Ename	Hipshookup
		Type	local
		At	Hips

		
	End
##

	Event	
		Ename	WepRhookup
		Type	local
		At	WepR

		
	End

	Event	
		Ename	WepLhookup
		Type	local
		At	WepL

		
	End

	Event	
		Ename	LlegRhookup
		Type	local
		At	LlegR

		
	End
##

	Event	
		Ename	LlegLhookup
		Type	local
		At	LlegL

		
	End

	Event	
		Ename	UlegRhookup
		Type	local
		At	UlegR

		
	End

	Event	
		Ename	UlegLhookup
		Type	local
		At	UlegL

		
	End

##

	Event	
		Ename	FootLhookup
		Type	local
		At	FootL

		
	End

	Event	
		Ename	FootRhookup
		Type	local
		At	FootR

		
	End


End

Condition
	On 	cycle
	Time 	3
	Chance	.15

	Event
		Type	Local 
		At	headhookup
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
		At	neckhookup
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
		At	chesthookup
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
		At	Hipshookup
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
		At	UARMLhookup
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
		At	UARMRhookup
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
		At	LARMRhookup
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
		At	LARMLhookup
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
		At	WepLhookup
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
		At	WepRhookup
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
		At	UlegRhookup
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
		At	ULEGLhookup
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
		At	LLEGLhookup
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
		At	LLEGRhookup
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
		At	FOOTLhookup
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
		At	FOOTRhookup
		
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	10
	End

End

End		