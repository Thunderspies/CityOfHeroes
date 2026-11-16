
FxInfo

Flags    InheritAlpha
##################################



#######  Crate  #############################################




Condition
	On 	Time
	Time 	52



	Event
		Type	Local
		At	origin
		Sound rockthrow 60 60 .33
		Sound rockthrow 60 60 .27
		Sound rockthrow 60 60 .3

		
	End

End

Condition
	On 	Time
	Time 	69



	Event
		Type	Local
		At	origin
		Sound coalhit 60 60 .77
		
	End

End


Condition
	On 	Time
	Time 	0



	Event
		EName	Prop
		Type	Local
		At	BendMystic
		Geom	WepR_Crate2_Sm_Wood
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		
		Lifespan	73
	End

End

######### Crate Hit Ground  ##################################

Condition
	On 	Time
	Time 	73

	Event
		EName 	Prime1
		Type	Start
		At	BendMystic
		
		Part	Explosions\WoodExplosion\WoodExplosion.part
		Part	Explosions\WoodExplosion\WoodExplosion01.part
		Part	Explosions\WoodExplosion\WoodExplosion02.part

		

	End



End

Condition
	On 	Time
	Time 	73

	Event
		EName 	Prime
		Type	Start 
		At	BendMystic
		Part	Explosions\WoodExplosion\SawDust01.part
		Part	Explosions\WoodExplosion\SawDust02.part
		Part	Explosions\WoodExplosion\SawdustLarge.part
		Sound exploblast 60.0 60.0 .85
		Sound quills4 60.0 60.0 .46
		Sound exploblast 60.0 60.0 .79
		Sound explohit2 60.0 60.0 .68
	End

End

Condition
	Time 	73
	Chance	.9

	Event
		Type	Start 
		At	BendMystic
		Geom	WoodPlank01
		Geom	WoodPlank02
		Geom	WoodPlank03
		Geom	WoodPlank05
		Bhvr	Behaviors\StartOffset.bhvr
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	130
	End

End

Condition
	Time 	73
	Chance	.9

	Event
		Type	Start 
		At	BendMystic
		Geom	WoodPlank01
		Geom	WoodPlank02
		Geom	WoodPlank03
		Geom	WoodPlank05
		Bhvr	Behaviors\StartOffset2.bhvr
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	130
	End

End

Condition
	Time 	73
	Chance	.9

	Event
		Type	Start 
		At	BendMystic
		Geom	WoodPlank01
		Geom	WoodPlank02
		Geom	WoodPlank03
		Geom	WoodPlank05
		Bhvr	Behaviors\StartOffset3.bhvr
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	130
	End

End

Condition
	Time 	73
	Chance	.9

	Event
		Type	Start 
		At	BendMystic
		Geom	WoodPlank01
		Geom	WoodPlank02
		Geom	WoodPlank03
		Geom	WoodPlank05
		Bhvr	Behaviors\StartOffset4.bhvr
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	130
	End

End

Condition
	Time 	73
	Chance	.9

	Event
		Type	Start 
		At	BendMystic
		Geom	WoodPlank01
		Geom	WoodPlank02
		Geom	WoodPlank03
		Geom	WoodPlank05
		Bhvr	Behaviors\StartOffset5.bhvr
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	130
	End

End

Condition
	On 	Time
	Time	79

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	76

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	77

	Event
		EName 	Prime
		Type	Destroy
	End

End	

Condition
	On 	Time
	Time 	160  ##233

	Event
		EName 	All
		Type	Destroy 
	End


End




End				