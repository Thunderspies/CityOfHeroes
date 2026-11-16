#########################################################
##	Hilcopter Rig
#########################################################

FxInfo

Flags InheritAlpha


############## sound  ###############################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fuselage
		Type	Local
		At	Root
		BhvrOverride
			Scale 1 1 1
		End
		#Geom	_Helo01_L01__Helo1
		Sound HelicopterTurbine_Loop 150 150 .88
	End

###############  Main rotor   #############################################

	Event
		EName 	MainRotor	##(anim, minus geometry)
		Type	Local
		At	Fuselage
		BhvrOverride
			PositionOffset 0 15 14
			pyrRotate	-11 0 0
			Scale		.58 .58 .58
		End
		Anim	HelicopterBlade
	End

	Event
		EName	MainRotor_node ##(refrence note from Anim)
		Type	local
		At	MainRotor
		AnimPiv	Hips
		Sound HelicopterMainRotor_Loop 600 250 1
	End

	Event
		EName 	MainRotor_geo
		Type	Local
		At	MainRotor_node
		Geom	_Helo01_MainRotor_Blur
		Sound Tutorial_Helicopter_Loop 1500 1000 .5
	End


############    Tail Rotor    #############################################



	Event
		EName 	TailRotor	##(anim, minus geometry)
		Type	Local
		At	Fuselage
		BhvrOverride
			PositionOffset 0 13.5 -12.5
			pyrRotate	0 0 90
			Scale		.3 .3 .3
		End
		Anim	HelicopterBlade
	End

	Event
		EName	TailRotor_node ##(reference note from Anim)
		Type	local
		At	TailRotor
		AnimPiv	Hips
	End

	Event
		EName 	TailRotor_geo
		Type	Local
		At	TailRotor_node
		Geom	_Helo01_TailRotor_Blur
		Sound HelicopterTailRotor_Loop 200 200 .88
	End

End