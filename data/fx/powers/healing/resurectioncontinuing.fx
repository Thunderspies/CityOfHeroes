#########################################################
##	chill touch hit
FxInfo

LifeSpan	600

Input  
	InpName	Hips
End

Input  
	InpName	FootL
End

Input  
	InpName	FootR
End

Input  
	InpName	ULEGL
End

Input  
	InpName	ULEGR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

Input  
	InpName	CHEST
End

Input  
	InpName	Hair
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	Root
End

Input  
	InpName	wepR
End

Input  
	InpName	wepL
End

########################################################

Condition
	On 	Time
	Time 	30


	Event
		EName	Left
		Type	Local 
		At	wepL
		
		Part1	Powers/Ability/Stamina2.part
		Part2	Powers/Ability/StaminaSmall2.part
		Part3	Powers/Ability/AbilityDots.part
		Part4	Powers/Ability/AbilitySparklingDots.part
		Sound heal5 70 70 .75
	End

	Event
		EName	Right
		Type	Local 
		At	wepR
		
		Part1	Powers/Ability/Stamina2.part
		Part2	Powers/Ability/StaminaSmall2.part
		Part3	Powers/Ability/AbilityDots.part
		Part4	Powers/Ability/AbilitySparklingDots.part
	End

End
############################################################################################

Condition
	On 	Time
	Time 	80

	Event
		EName 	BottleRocketHookup
		Type	Local 
		At	Root 
		Bhvr	Behaviors/teleportscale.bhvr
		Anim	FX_teleportbase
	End

	Event
		EName 	BottleRocket02
		Type	Local 
		At	BottleRocketHookup
		AnimPiv Waist
		Part1	Powers/Teleport/RessurectionBottleRocketSparks.part
		Part2	Powers/Teleport/BottleRocketHead.part
		#Part3	Powers/Teleport/TeleStreak.part
	End


	Event
		EName 	BottleRocket03
		Type	Local 
		At	BottleRocketHookup
		AnimPiv Neck
		Part1	Powers/Teleport/RessurectionBottleRocketSparks.part
		Part2	Powers/Teleport/BottleRocketHead.part
		#Part3	Powers/Teleport/TeleStreak.part
	End

	Event
		EName 	BottleRocket04
		Type	Local 
		At	BottleRocketHookup
		AnimPiv Head
		Part1	Powers/Teleport/RessurectionBottleRocketSparks.part
		Part2	Powers/Teleport/BottleRocketHead.part
		#Part3	Powers/Teleport/TeleStreak.part

	End

	Event
		EName	Outerglow
		Type	Local
		At	Root
		
		Part1	Powers/Teleport/Sparklies.part
		Part2	Powers/Teleport/RingSparklies.part
		Part3	Powers/Teleport/SharpRingSparklies.part
		Part4	Powers/Teleport/streaks.part
		Part5	Powers/Teleport/Glowstreaks.part
	End

End

#############################################################################################

Condition
	On	Time
	Time	78
	
	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers/Ability/StaminaRingUp.part
		#Sound	heal1 70 25 .8
		
	End

End

Condition
	On	Time
	Time	85
	
	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers/Ability/StaminaRingUp.part
		
	End

End

Condition
	On	Time
	Time	95
	
	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers/Ability/StaminaRingUp.part
		
	End

End

############################################################################################

Condition
	On 	Time
	Time 	40

	Event
		EName	1
		Type	Local
		At	UlegL
		part1	Powers/Regeneration/RegenBodyGlows2.part
		PMagnet	Hips
	End

	Event
		EName	2
		Type	Local
		At	UlegR
		part1	Powers/Regeneration/RegenBodyGlows2.part
		PMagnet	Hips
	End

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	Powers/Regeneration/RegenBodyGlows2.part
		PMagnet	ULEGL

	End

	Event
		EName	4
		Type	Local
		At	LlegR
		part1	Powers/Regeneration/RegenBodyGlows2.part
		PMagnet	ULEGR


	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	Powers/Regeneration/RegenBodyGlows3.part
		PMagnet	LLEGL

	End

	Event
		EName	6
		Type	Local
		At	FootR
		part1	Powers/Regeneration/RegenBodyGlows3.part
		PMagnet	LLEGR

	End

####################################################################################
##Original
##RegenBodyGlows2.part
#####################################################################################
	
	Event
		EName	7
		Type	Local
		At	FootR
		part1	Powers/Regeneration/RegenBodyGlows.part
		PMagnet LlegR
	End


	Event
		EName	8
		Type	Local
		At	ULEGL
		part1	Powers/Regeneration/RegenBodyGlows.part
		

	End

	Event
		EName	9
		Type	Local
		At	LLEGL
		part1	Powers/Regeneration/RegenBodyGlows.part
		
		PMagnet UlegL
	End

	Event
		EName	10		
		Type	Local
		At	FootL
		part1	Powers/Regeneration/RegenBodyGlows.part
		PMagnet LlegL
	End



	Event
		EName	11
		Type	Local
		At	ULEGR
		part1	Powers/Regeneration/RegenBodyGlows.part
		

	End


#LegR


	Event
		EName	12
		Type	Local
		At	LLEGR
		part1	Powers/Regeneration/RegenBodyGlows.part
		
		PMagnet UlegL
	End

###############################################################################

	Event
		EName	13
		Type	Local
		At	Head
		part1	Powers/Regeneration/RegenBodyGlows.part
		
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	14
		Type	Local
		At	UArmL
		part1	Powers/Regeneration/RegenBodyGlows.part
		
		#PMagnet ElbowL

	End

	Event
		EName	15
		Type	Local
		At	LArmL
		part1	Powers/Regeneration/RegenBodyGlows.part
		
		#PMagnet HandL
	End

	Event
		EName	16
		Type	Local
		At	WepL
		part1	Powers/Regeneration/RegenBodyGlows.part
	
	End

	Event
		EName	17
		Type	Local
		At	UArmR
		part1	Powers/Regeneration/RegenBodyGlows.part
		
		#PMagnet ElbowL

	End

	Event
		EName	18
		Type	Local
		At	LArmR
		part1	Powers/Regeneration/RegenBodyGlows.part
		
		#PMagnet HandL
	End

	Event
		EName	19
		Type	Local
		At	WepR
		part1	Powers/Regeneration/RegenBodyGlows.part
	
	End

End
###################################################################################

Condition
	On	Time
	Time	100
	
	Event
		EName	Left
		Type	Destroy
				
	End

	Event
		EName	Right
		Type	Destroy
				
	End

End

Condition
	On	Time
	Time	140
	
	Event
		EName	BottleRocketHookup
		Type	Destroy
				
	End

	Event
		EName	BottleRocket02
		Type	Destroy
				
	End

	Event
		EName	BottleRocket03
		Type	Destroy
				
	End

	Event
		EName	BottleRocket04
		Type	Destroy
				
	End

End


Condition
	On	Time
	Time	120

	Event
		EName	OuterGlow
		Type	Destroy
				
	End

End

End				