#########################################################
##	template

FxInfo

Input  
	Inpname	Root
End

LifeSpan	80

#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	BottleRocketHookup
		Type	start 
		At	Root 
		Bhvr	Behaviors/teleportscale.bhvr
		Anim	FX_teleportbase

		LifeSpan	33

	End

	Event
		EName 	BottleRocket02
		Type	local 
		At	BottleRocketHookup
		AnimPiv Waist
		Part	:BottleRocketSparks.part
		Part	:BottleRocketSparksDark.part
		Part	:BottleRocketHead.part
		Part	:BottleRocketglow.part

		LifeSpan	24
	End


	Event
		EName 	BottleRocket03
		Type	local 
		At	BottleRocketHookup
		AnimPiv Neck
		Part	:BottleRocketSparks.part
		Part	:BottleRocketSparksDark.part
		Part	:BottleRocketHead.part
		Part	:BottleRocketglow.part
		LifeSpan	33
	End

	Event
		EName 	BottleRocket04
		Type	local 
		At	BottleRocketHookup
		AnimPiv Head
		Part	:BottleRocketSparks.part
		Part	:BottleRocketSparksDark.part
		Part	:BottleRocketHead.part
		Part	:BottleRocketglow.part
		LifeSpan	33

	End

End

###################################################################################################

Condition
	On 	Time
	Time 	33
	

	Event
		EName 	BottleRocketHookup2
		Type	start 
		At	Root 
		Bhvr	Behaviors/teleportscale.bhvr
		Anim	FX_teleportbase

		LifeSpan	33

	End

	Event
		EName 	BottleRocket02
		Type	local 
		At	BottleRocketHookup2
		AnimPiv Waist
		Part	:BottleRocketSparks.part
		Part	:BottleRocketSparksDark.part
		Part	:BottleRocketHead.part
		Part	:BottleRocketglow.part
		LifeSpan	24
	End


	Event
		EName 	BottleRocket03
		Type	local 
		At	BottleRocketHookup2
		AnimPiv Neck
		Part	:BottleRocketSparks.part
		Part	:BottleRocketSparksDark.part
		Part	:BottleRocketHead.part
		Part	:BottleRocketglow.part
		LifeSpan	33
	End

	Event
		EName 	BottleRocket04
		Type	local 
		At	BottleRocketHookup2
		AnimPiv Head
		Part	:BottleRocketSparks.part
		Part	:BottleRocketSparksDark.part
		Part	:BottleRocketHead.part
		Part	:BottleRocketglow.part
		LifeSpan	33

	End

	Event
		EName	Outerglow
		Type	start
		At	Root
		
		Part	:Sparklies.part
		Part	:sparkliesDark.part
		Part	:RingSparklies.part
		Part	:SharpRingSparklies.part
		Part	:streaks.part
		Part	:GlowstreaksBlack.part
	End

End

Condition
	On 	Time
	Time 	2
	
	Event
		EName	innerGlow
		Type	start
		At	Root
		
		Part	:RingSparklies.part
		Part	:SharpRingSparklies.part
		Part	:streaks.part
		Part	:Glowstreaks.part
	End

End
	

Condition
	On 	Time
	Time 	0
	
	Event
		EName	innerGlow
		Type	start
		At	Root
		
		Part	:ringout.part
		
		LifeSpan	60
	End

End
	
	
Condition
	On 	Time
	Time 	3
	
	Event
		EName	innerGlow
		Type	start
		At	Root
		
		Part	:ringoutRed.part
		LifeSpan	60
	End

End
	
End		