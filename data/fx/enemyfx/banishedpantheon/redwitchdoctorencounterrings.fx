#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	root
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

LifeSpan	120

############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	Hand1
		Type	Local 
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		LifeSpan	60
		Sound buildup 66 66 .33
		
	End

	Event
		EName	Hand2
		Type	Local 
		At	WepL
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
		
		LifeSpan	60
		
	End
	
End

############################################################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	BottleRocketHookup
		Type	Local 
		At	Root 
		Bhvr	Behaviors/RadiationPoisoningScale.bhvr
		Anim	FX_teleportbase
		#Sound	accel2_loop 66 22 .6
		
	End

	Event
		EName 	sound1
		Type	Local 
		At	root
		sound	accel5 66 22 .9
		
	End
	
	Event
		EName 	BottleRocket03
		Type	Local 
		At	BottleRocketHookup
		AnimPiv Neck
		Part1	:Poisoningtrail.part
		Part2	:PoisoningHead.part
		Part3	:RadiationHandElectrons.part
		Lifespan	75
		
	End

	Event
		EName 	BottleRocket04
		Type	Local 
		At	BottleRocketHookup
		AnimPiv Head
		Part1	:Poisoningtrail.part
		Part2	:PoisoningHead.part
		Part3	:RadiationHandElectrons.part
		Lifespan	75

	End

	Event
		EName	HookUp
		Type	Local
		At	Root
		Geom	RootToHeadAdjustment

	End

	Event
		EName	RingsDown
		Type	Local
		At	HookUp
		Altpiv	1

		Part1	:RadiationPoisoningRingsDown.part

	End

	Event
		EName	RingsOut
		Type	Local
		At	Root

		Part1	:RadiationPoisoningRingsOut.part

	End
End

End			