#########################################################
##	template

FxInfo


###########  Electricity   ##########################

Condition
	On Time 
	Time 0
	
	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	POWERS\ElectricityControl\ElectricityControl01.part
		Part2	POWERS\ElectricityControl\ElectricityControl02.part
		Part3	POWERS\ElectricityControl\ElectricityInnerGlow.part
		Sound eleccontrol2 50 50 .4
		

	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part1	POWERS\ElectricityControl\ElectricityControl01.part
		Part2	POWERS\ElectricityControl\ElectricityControl02.part
		Part3	POWERS\ElectricityControl\ElectricityInnerGlow.part
		
	End


	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part1	POWERS\ElectricityControl\ElectricityOuterGlow.part
		Part2	POWERS\ElectricityControl\ElectricityArch.part
		Part3	POWERS\ElectricityControl\ElectricityRing.part
		Part4	POWERS\ElectricityControl\ElectricityArchInvert.part
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		Part1	POWERS\ElectricityControl\ElectricityOuterGlow.part
		Part2	POWERS\ElectricityControl\ElectricityArch.part
		Part3	POWERS\ElectricityControl\ElectricityRing.part
		Part4	POWERS\ElectricityControl\ElectricityArchInvert.part
		
	End

##############  Speed  #################3

	Event
		EName	Right
		Type 	Local
		At	WepR
		#Part1	POWERS\SuperSpeed\speedattack1.part
		Sound flurrypower 50 50 .53
	End

	Event
		Ename	Left
		Type	Local
		At	WepL
		#Part1	POWERS\SuperSpeed\speedattack1.part
	End
	
	Event
		EName	Right2
		Type 	Posit
		At	WepR
		Part1	POWERS\SuperSpeed\SpeedAttackStreaks.part
		#Part2	POWERS\SuperSpeed\speedattack2.part
	End

	Event
		Ename	Left2
		Type	Posit
		At	WepL
		Part1	POWERS\SuperSpeed\SpeedAttackStreaks.part
		#Part2	POWERS\SuperSpeed\speedattack2.part
	End

	Event
		Ename	aura
		Type	local
		At	Chest
		Part2	POWERS\SuperSpeed\FlurryAura.part
		Part1	POWERS\SuperSpeed\FlurryAuraOrange.part
		BhvrOverride
			HueShift	-120
		End
		PMagnet	Chest

	End

End


Condition
	On Time 
	Time 3

	Event
		Ename	BubblingSpots
		Type	start
		At	Chest
		Part2	POWERS\SuperSpeed\BubblingSpots.part
	End

End

###################################################################


Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		Bhvr	behaviors/speed.bhvr
		At	Root
		Anim	Mblur
		BhvrOverride
			StartColor	0 0 255
		End

	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		Bhvr	behaviors/speed.bhvr
		At	Root
		Anim	Mblur
		BhvrOverride
			StartColor	0 0 255
		End
	End
End

Condition
	On 	Time
	Time 	6

	Event
		Type	Local
		Bhvr	behaviors/speed.bhvr
		At	Root
		Anim	Mblur
		BhvrOverride
			StartColor	0 0 255
		End
		Sound Flurry 50 50 .75
	End
End

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		Bhvr	behaviors/speed.bhvr
		At	Root
		Anim	Mblur
		BhvrOverride
			StartColor	0 0 255
		End
	End
End

Condition
	On 	Time
	Time 	83
	
	Event
		Type	Posit
		At	WepR
		EName	FistStar
		Part1	POWERS\SuperSpeed\SpeedAttackstar.part
		Sound whoosh13 80 80 .82
	End

End

Condition
	On 	Time
	Time 	83
	
	Event
		Type	Destroy
		EName	BubblingSpots
	End
End

Condition
	On 	Time
	Time 	90
	
	Event
		Type	Destroy
		EName	All
	End
End	

End		