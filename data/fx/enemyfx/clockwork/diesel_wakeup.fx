#########################################################
##	onfire
##
FxInfo  

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Start 
		At	Mystic
		
		Part	Explosions\MetalExplosion\MetalSparks.part
		Part	Explosions\MetalExplosion\MetalExplosionBig.part
		Part	Explosions\MetalExplosion\MetalExplosion01.part
		Part	Explosions\MetalExplosion\MetalExplosion02.part
		Part	Explosions\MetalExplosion\MetalBits.part
		Part	Explosions\MetalExplosion\MetalBits2.part
		Sound telekinetic3 80 80 .9

	End



End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Mystic
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Bhvr	Behaviors\MetalOffset.bhvr
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Mystic
		Geom	Broken_Scaffolding_1
		Geom	Broken_Scaffolding_2
		Geom	Broken_Scaffolding_3
		Geom	Broken_Scaffolding_4
		Bhvr	Behaviors\MetalOffset2.bhvr
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Mystic
		Geom	Broken_Scaffolding_1
		Geom	Broken_Scaffolding_2
		Geom	Broken_Scaffolding_3
		Geom	Broken_Scaffolding_4
		Bhvr	Behaviors\MetalOffset3.bhvr
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Mystic
		Geom	Broken_Scaffolding_1
		Geom	Broken_Scaffolding_2
		Geom	Broken_Scaffolding_3
		Geom	Broken_Scaffolding_4
		Bhvr	Behaviors\MetalOffset4.bhvr
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Mystic
		Geom	Broken_Scaffolding_1
		Geom	Broken_Scaffolding_2
		Geom	Broken_Scaffolding_3
		Geom	Broken_Scaffolding_4
		Bhvr	Behaviors\MetalOffset5.bhvr
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	130
	End

End

Condition
	On 	Time
	Time	9

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	3

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	7

	Event
		EName 	Prime
		Type	Destroy
	End

End	

Condition
	On 	Time
	Time 	120

	Event
		EName 	All
		Type	Destroy 
	End


End


End			