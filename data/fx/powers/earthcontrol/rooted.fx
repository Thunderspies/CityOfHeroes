#########################################################
##	
##
FxInfo

#Flags	IsArmor

Flags    InheritAlpha

##########################################################

Condition
	On	Time
	Time 	0
		
	Event
		Type	Local
		At 	origin
		Sound Stone6 100 100 .88
	End
End

Condition
	On	Time
	Time 	5
		
	Event
		Type	Local
		At 	origin
		Sound Stone_loop 100 100 .5
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	FootL
		Bhvr 	:RootedFoot.bhvr
		geom	MineralArmor_FootL
		

	End

	Event
		Type	Local 
		At	FootR
		Bhvr 	:RootedFootR.bhvr
		geom	MineralArmor_FootR
		
	End

	Event
		Type	Local 
		At	LLEGL
		Bhvr 	:RootedOpposite.bhvr
		geom	MineralArmor_LLEGR
		Part	:StoneArmorRocks.part
		Part	:StoneArmorDirt01.part
		Part	:StoneArmorDirt02.part
	End

	Event
		Type	Local 
		At	LLEGR
		Bhvr 	:RootedOppositeL.bhvr
		geom	MineralArmor_LLEGL
		Part	:StoneArmorRocks.part
		Part	:StoneArmorDirt01.part
		Part	:StoneArmorDirt02.part
	End

End



###################################################################

Condition
	On 	death
	
	Event
		Ename	FinalRocks
		Type	start
		At	LLEGL
		#Part1	:RocksScatter2.part
		Part2	:RocksScatter.part
		Sound graniteexplo 70 70 .66
		LifeSpan	19
		
	End
	
	

	Event
		Ename	FinalDust
		Type	start
		At	LLEGL
		Part3	:StoneArmordirtExplosion.part
		Part3	:StoneArmordirtExplosion1.part
		LifeSpan	7
	End

	Event
		Ename	FinalRocks
		Type	start
		At	LLEGR
		#Part1	:RocksScatter2.part
		Part2	:RocksScatter.part
		LifeSpan	19
		
	End
	
	

	Event
		Ename	FinalDust
		Type	start
		At	LLEGR
		Part3	:StoneArmordirtExplosion.part
		Part3	:StoneArmordirtExplosion1.part
		LifeSpan	7
	End

End

End	