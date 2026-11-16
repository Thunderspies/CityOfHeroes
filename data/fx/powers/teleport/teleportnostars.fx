#########################################################
## Teleport - No Stars
#########################################################

FxInfo

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
		LifeSpan	77
	End

	Event
		EName 	BottleRocket02
		Type	local 
		At	BottleRocketHookup
		AnimPiv Waist
#		Part1	:BottleRocketSparks.part
#		Part2	:BottleRocketHead.part
#		#Part3	:TeleStreak.part
		LifeSpan	77
	End

	Event
		EName 	BottleRocket03
		Type	local 
		At	BottleRocketHookup
		AnimPiv Neck
#		Part1	:BottleRocketSparks.part
#		Part2	:BottleRocketHead.part
#		#Part3	:TeleStreak.part
		LifeSpan	77
	End

	Event
		EName 	BottleRocket04
		Type	local 
		At	BottleRocketHookup
		AnimPiv Head
#		Part1	:BottleRocketSparks.part
#		Part2	:BottleRocketHead.part
#		#Part3	:TeleStreak.part
		LifeSpan	77
	End

	Event
		EName	glow
		Type	start
		At	Root
		Geom	teleport
		bhvr	behaviors/Scale5.bhvr
	End

	Event
		EName	Outerglow
		Type	start
		At	Root
		Part1	:Sparklies.part
		Part2	:RingSparklies.part
		Part3	:SharpRingSparklies.part
		Part4	:streaks.part
		Part5	:Glowstreaks.part
	End
End

Condition
	On 	Time
	Time 	2
	
	Event
		EName	innerGlow
		Type	start
		At	Root
		Part1	:ringout.part
		Part2	:RingSparklies.part
		Part3	:SharpRingSparklies.part
		Part4	:streaks.part
		Part5	:Glowstreaks.part
	End
End

#########################################################

End		