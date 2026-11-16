#########################################################
## Darkness Control - Tar Patch (Continuing FX) - Subtractive
#########################################################

FxInfo

Lifespan 900

#########################################################

Condition
	On	Time
	Time	30

	Event
		Type	posit
		At	root
		Sound forcefield5_loop 70 70 .6
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
	
	Event	
		ENAME	hookupW/Spin
		Type	posit
		At	T_root
		BHVR	:SplatRotate.bhvr
		Sound Tar_loop 80 80 .53
	End
End

Condition
	On 	Time
	Time 	30

	Event	
		ENAME	BaseTrail
		Type	posit
		At	root
		Part1	:CreateDarknessTrail.part
		Part2	:CreateDarknessTrail2.part
		Part3	:CreateDarknessPuddleFlat.part
		Part4	:CreateDarknessPuddle2Flat.part
		#Part5	:DarkTarRing.Part
		#Sound	forcefield5_loop 80 30 .7
	End

	Event
		EName	darkskirt
		Type	Posit #Start
		At	root
		Part	:SubtractiveFlash1.part
		Part	:CreateDarknessPuddle_Subtractive_Local.part
		Part1	:CreateDarknessPuddle_Subtractive.part
		Part2	:CreateDarknessPuddle2_Subtractive.part
	End

	Event	
		ENAME	BaseTrail
		Type	Posit
		At	root
		Part1	:CreateDarknessTrail_Subtractive.part
		Part2	:CreateDarknessTrail2_Subtractive.part
	End

	Event
		EName	darkskirt
		Type	Posit #Start
		At	root
		Part1	:CreateDarknessPuddleFlat_Subtractive.part
		Part2	:CreateDarknessPuddle2Flat_Subtractive.part
	End
End

#########################################################

End			