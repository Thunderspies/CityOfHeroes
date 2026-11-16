#########################################################
##	jack
##

FxInfo

Input  
	InpName	WepR
End

Flags    InheritAlpha

Condition
	On 	Time
	Time 	9

	Event
		EName 	Axe
		Type	Local 
		At	WepR
		Geom	GEO_WepR_Infernal
		Sound maceout 70 70 .7
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End

###################### Handle ##########################

	Event
		EName 	CenterNode
		Type	Local 
		At	Axe
		Bhvr	:FireOffset1.bhvr
		Sound torch1_loop 48 48 .59
	End

	Event
		EName 	CenterNodeFire
		Type	Local
		At	wepR
		POther	CenterNode
		#Part	:AxeHandleFire01.part
		Part	:AxeHandleGlow.part
		Part	:AxeHandleFire02.part
		Bhvr	:HandleOffset1.bhvr
		
	End

######################Right Side##########################

	Event
		EName 	Node2
		Type	Local 
		At	Axe
		Part	:AxeFire02.part
		POther	Node3
		Bhvr	:FireOffset2.bhvr
	End

	Event
		EName 	Node3
		Type	Local 
		At	Axe
		#Part	:AxeFire02.part
		Bhvr	:FireOffset3.bhvr
	End

	Event
		EName 	Node4
		Type	Local 
		At	Axe
		Part	:AxeFire02.part
		POther	Node2
		Bhvr	:FireOffset4.bhvr
	End

######################Left  Side##########################

	Event
		EName 	Node5
		Type	Local 
		At	Axe
		Part	:AxeFire02.part
		POther	Node6
		Bhvr	:FireOffset5.bhvr
	End

	Event
		EName 	Node6
		Type	Local 
		At	Axe
		#Part	:AxeFire02.part
		Bhvr	:FireOffset6.bhvr
	End

	Event
		EName 	Node7
		Type	Local 
		At	Axe
		Part	:AxeFire02.part
		POther	Node5
		Bhvr	:FireOffset7.bhvr
	End
End

End		