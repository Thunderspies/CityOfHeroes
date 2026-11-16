#########################################################
##	template

FxInfo

Flags    InheritAlpha
#########################################################

Condition
	On	Time
	Time	5

	Event	
		Type	local
		At	Hips	
		Sound Ghostship1_loop 800 800 .95
	End
End


Condition
	On 	Time
	Time 	10	
	
	Event
		EName 	Ghost_Ship
		Type	local
		At	Hips
		GEOM	Ghost_Ship_01
		Bhvr	:Ghost_Ship_Color.bhvr
	End	
	Event
		EName 	Ghost_Ship
		Type	local
		At	Hips
		GEOM	Ghost_Ship_02
		Bhvr	:Ghost_Ship_Color.bhvr
	End	
	Event
		EName 	Ghost_Ship
		Type	local
		At	Hips
		GEOM	Ghost_Ship_03
		Bhvr	:Ghost_Ship_Color.bhvr
	End	
End
