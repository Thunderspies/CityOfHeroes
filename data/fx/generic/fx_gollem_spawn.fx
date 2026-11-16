#########################################################
##	template

FxInfo

Input  
	InpName	Root
End

Input  
	InpName	Chest
End

Input  
	InpName	UarmR
End

Input  
	InpName	UarmL
End

Input  
	InpName	Hips
End

Input  
	InpName	UlegR
End

Input  
	InpName	UlegL
End

Input
	InpName Origin
End

Lifespan	140


###########################################################
################## Animation ##############################
###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	AnimationNode
		Type	Start
		At	Root
		Anim	FX_gollem_spawn
		Bhvr	EnemyFX/Clockwork/GeoFadeUpAndOut.bhvr
		LifeSpan 100
	End

	Event
		ENAME	HipsNode
		Type	Local
		At	AnimationNode
		Part	Powers/EarthControl/GollemStoneArmorRocks.part
		Part	Powers/EarthControl/GollemStoneArmorDirt01a.part
		Part	Powers/EarthControl/GollemStoneArmorDirt02a.part
		AnimPiv Hips	
	End

	Event
		ENAME	ChestNode
		Type	Local
		At	AnimationNode
		Part	Powers/EarthControl/GollemStoneArmorRocks.part
		Part	Powers/EarthControl/GollemStoneArmorDirt01a.part
		Part	Powers/EarthControl/GollemStoneArmorDirt02a.part
		AnimPiv Chest	
	End

	Event
		ENAME	LarmRNode
		Type	Local
		At	AnimationNode
		Part	Powers/EarthControl/GollemStoneArmorRocks.part
		Part	Powers/EarthControl/GollemStoneArmorDirt01a.part
		Part	Powers/EarthControl/GollemStoneArmorDirt02a.part
		AnimPiv UarmR	
	End

	Event
		ENAME	UarmLNode
		Type	Local
		At	AnimationNode
		Part	Powers/EarthControl/GollemStoneArmorRocks.part
		Part	Powers/EarthControl/GollemStoneArmorDirt01a.part
		Part	Powers/EarthControl/GollemStoneArmorDirt02a.part
		AnimPiv UarmL	
	End

	Event
		ENAME	LlegRNode
		Type	Local
		At	AnimationNode
		Part	Powers/EarthControl/GollemStoneArmorRocks.part
		Part	Powers/EarthControl/GollemStoneArmorDirt01a.part
		Part	Powers/EarthControl/GollemStoneArmorDirt02a.part
		AnimPiv UlegR	
	End

	Event
		ENAME	LlegLNode
		Type	Local
		At	AnimationNode
		Part	Powers/EarthControl/GollemStoneArmorRocks.part
		Part	Powers/EarthControl/GollemStoneArmorDirt01a.part
		Part	Powers/EarthControl/GollemStoneArmorDirt02a.part
		AnimPiv UlegL	
	End

End


Condition
	On 	Time
	Time 	0

	Event
		EName   Wind
		Type	start 
		At	Root
		Bhvr 	powers/coldcontrol/IceArmorWinda.bhvr
		geom	FX_AirWisp01	#FX_TmpCtrlWisp01
		#part1	:IceMistUpwards.part
		#part2	:SnowMistUpwards.part
		#Sound	blizzard 80 30 .65
		LifeSpan	73
	End
End

Condition
	On 	Time
	Time 	5
	Event
		EName   WindFast
		Type	start 
		At	Root
		Bhvr 	powers/coldcontrol/IceArmorWindFasta.bhvr
		geom	FX_AirWisp01 #FX_TmpCtrlWisp01
		LifeSpan	75
	End
End

Condition
	On 	Time
	Time 	10
	Event
		EName   WindFirst
		Type	start 
		At	Root
		Bhvr 	powers/coldcontrol/IceArmorWindFirsta.bhvr
		geom	FX_AirWisp01	#FX_TmpCtrlWisp01
		LifeSpan	80
	End
End

Condition
	On 	Time
	Time 	98

	Event
		Ename	FinalRocks
		Type	local
		At	AnimationNode
		AnimPiv	Chest
		Part1	Powers/EarthControl/RocksScatter2.part
		Part2	Powers/EarthControl/RocksScatter.part
		LifeSpan	19
		Part1	Powers/EarthControl/Flash.part
		
	End
	
	

	Event
		Ename	FinalDust
		Type	local
		At	AnimationNode
		AnimPiv	Chest
		Part3	Powers/EarthControl/StoneArmordirtExplosionx.part
		Part1	Powers/EarthControl/StoneArmordirtExplosion1x.part
		LifeSpan	17
	End

	Event
		Ename	RockExplosion
		Type	local
		At	AnimationNode
		AnimPiv	chest
		Part3	Powers/EarthControl/RockExplosion01.part
		Part2	Powers/EarthControl/RockExplosion02.part
		LifeSpan	17
	End

End


End