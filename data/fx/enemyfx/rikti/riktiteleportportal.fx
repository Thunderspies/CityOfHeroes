#########################################################
##	template

FxInfo

Input  
	InpName	Root
End

Input  
	InpName	Hair
End

Input  
	InpName	Eyes
End

Input  
	InpName	Neck
End

Input  
	InpName	Chest
End

Input  
	InpName	SpadR
End

Input  
	InpName	SpadL
End

Input  
	InpName	UarmR
End

Input  
	InpName	UarmL
End

Input  
	InpName	LarmR
End

Input  
	InpName	LarmL
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
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
	InpName	LlegR
End

Input  
	InpName	LlegL
End

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	ToeL
End

Input  
	InpName	ToeR
End

Input
	InpName Origin
End 

Flags    InheritAlpha


Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################

Condition

	On 	Time	
	Time	18

	Event
		Type Local
		At origin
		Sound teletrans4 80 80 .88
	End

End

Condition

	On 	Time	
	Time	70

	Event
		Type Local
		At origin
		Sound teletrans3 80 80 .78
	End

End

#########################################################
################## WARP OUT FLASH #######################
#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	Hook
		Type	local
		At	root
		Geom	OverHeadDummy01
	
	End

	Event
		Ename	Hookup
		Type	start
		At	root
		Geom	RootToChestAdjustment
		
	End

	Event
		EName	Ring
		Type	local
		At	Hookup
		altpiv	1
		Part1	:RiktiPortalEllipseA.part
		Lifespan 50
		
	End

	Event
		Ename	Upwards
		Type	local
		At	hookup
		altpiv	1
		Part1	:UpwardStreaks.part
		Lifespan 50
	End

	Event
		Ename	elec
		Type	local
		At	chest
		
		Part2	:RiktiPortalSpark.part
		Lifespan 50

	End

End

Condition
	On	Time
	Time	2

	Event
		EName	AntiMatter
		Type	local
		At	Hookup
		altpiv	1
		Part3	:RiktiPortalSpecksA.part
		Part4	:RiktiPortalStreaksA.part
		Sound Materialize1 500.0 500.0 .5
		Part2	:RiktiPortalRingsA.part
		Part2	:PortalRaysA.part
		Part3	:PortalStreakRaysA.part
		Lifespan 50
	End

End

Condition
	On	Time
	Time	8
	Event
		EName	Ring2
		Type	local
		At	Hookup
		altpiv	1
		Part3	:RiktiPortalEllipsePulseA.part
		Sound glow6_loop 80 80 .7
		Lifespan 50
		
	End

End


Condition
	On	Time
	Time	16

	Event
		EName	Ring4
		Type	local
		At	Hookup
		altpiv	1
		Part3	:RiktiEllipsePulseShrinkAnglesA.part
		Lifespan 50
	End

End


#########################################################
################## WARP IN FLASH ########################
#########################################################

Condition
	On	Time
	Time	60
		
	Event
		EName	Ring
		Type	local
		At	Hookup
		altpiv	1
		Part1	:RiktiPortalEllipse.part
		Lifespan 40
	End

	Event
		Ename	Upwards
		Type	local
		At	hookup
		altpiv	1
		Part1	:UpwardStreaks.part
		Lifespan 40
	End

	Event
		Ename	elec
		Type	local
		At	chest
		
		Part2	:RiktiPortalSpark.part
		Lifespan 40
	End

End

Condition
	On	Time
	Time	62

	Event
		EName	AntiMatter
		Type	local
		At	Hookup
		altpiv	1
		Part3	:RiktiPortalSpecks.part
		Part4	:RiktiPortalStreaks.part
		Sound Materialize1 500.0 500.0 .5
		Part2	:RiktiPortalRings.part
		Part2	:PortalRays.part
		Part3	:PortalStreakRays.part
		Lifespan 40
	End

End

Condition
	On	Time
	Time	68
	Event
		EName	Ring2
		Type	local
		At	Hookup
		altpiv	1
		Part3	:RiktiPortalEllipsePulse.part
		Sound glow6_loop 80 80 .7
		Lifespan 40
		
	End

End


Condition
	On	Time
	Time	76

	Event
		EName	Ring4
		Type	local
		At	Hookup
		altpiv	1
		Part3	:RiktiEllipsePulseShrinkAngles.part
		Lifespan 40
	End

End

End		