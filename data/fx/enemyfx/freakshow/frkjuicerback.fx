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
		At	Emblem
		Geom	FRK_Juicer_Back	
		Sound Jacobs1_loop 70 70 .6
	End

	Event
		EName	Pivot01
		Type	local
		At	Backpack
		AltPiv	1
	End

	Event
		EName	Pivot02
		Type	local
		At	Backpack
		AltPiv	2
	End

	Event
		EName	Pivot03
		Type	local
		At	Backpack
		AltPiv	3
	End

	Event
		EName	Pivot04
		Type	local
		At	Backpack
		AltPiv	4
	End

	Event
		EName	Pivot05
		Type	local
		At	Backpack
		AltPiv	5
	End

	Event
		EName	Pivot06
		Type	local
		At	Backpack
		AltPiv	6
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

	Event
		EName	GlowL
		Type	local
		At	HandL
		POther	LarmL
		Part	:FRKJuicerBackElectricityGlowArm01.part
	End

	Event
		EName	GlowR
		Type	local
		At	HandR
		POther	LarmR
		Part	:FRKJuicerBackElectricityGlowArm01.part
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

