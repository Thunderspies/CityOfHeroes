#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Emblem
End

Input  
	Inpname	LarmL
End

Input  
	Inpname	LarmR
End

Input  
	Inpname HandL
End

Input  
	Inpname	HandR
End


Flags    InheritAlpha

########################################################################


Condition
	On 	Time
	Time 	0

	Event
		EName	Backpack
		Type	local
		At	Chest
		##Geom	FRK_Juicer_Back
		##Geom	Mastermind_Bot_ProtectorArcs
		Sound Jacobs1_loop 15 15 .19
	End

	Event
		EName	Pivot01
		Type	local
		At	Backpack
		##AltPiv	1
		Bhvr	:ArcPivot_01.bhvr
	End

	Event
		EName	Pivot02
		Type	local
		At	Backpack
		##AltPiv	2
		Bhvr	:ArcPivot_02.bhvr
	End

	Event
		EName	Pivot03
		Type	local
		At	Backpack
		##AltPiv	3
		Bhvr	:ArcPivot_03.bhvr
	End

	Event
		EName	Pivot04
		Type	local
		At	Backpack
		##AltPiv	4
		Bhvr	:ArcPivot_04.bhvr
	End

	Event
		EName	Pivot05
		Type	local
		At	Backpack
		##AltPiv	5
		Bhvr	:ArcPivot_05.bhvr
	End

	Event
		EName	Pivot06
		Type	local
		At	Backpack
		##AltPiv	6
		Bhvr	:ArcPivot_06.bhvr
	End
End



Condition
	On 	Time
	Time 	0

	Event
		EName	Electricity
		Type	local
		At	Pivot01
		Pmagnet	Pivot02
		Part	:FRKJuicerBackElectricity01.part
	End
End



Condition
	On 	Time
	Time 	0

	Event
		EName	GlowL
		Type	local
		At	Pivot03
		Pmagnet	Pivot04
		Part	:FRKJuicerBackElectricityGlowLeft03.part
		Part	:FRKJuicerBackElectricityGlowLeft01.part
		Part	:FRKJuicerBackElectricityGlowLeft02.part
	End

	Event
		EName	GlowR
		Type	local
		At	Pivot05
		Pmagnet	Pivot06
		Part	:FRKJuicerBackElectricityGlowRight03.part
		Part	:FRKJuicerBackElectricityGlowRight01.part
		Part	:FRKJuicerBackElectricityGlowRight02.part
	End
End

Condition
	On 	triggerbits
	Triggerbits	death

	Event
		EName	All
		Type	Destroy
	End

End
##################################

