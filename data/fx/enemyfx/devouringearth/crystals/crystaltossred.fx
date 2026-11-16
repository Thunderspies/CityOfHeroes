#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	200

Input  
	InpName	Origin
End


Input  
	InpName	Target
End

Input  
	InpName	WepR
End


###########################################################
##################################
##################################

Condition
	On 	Time
	Time 	18

	Event
		EName	Prime
		Type	start
		At	WepR
		Geom	Red_CrystalProjectile
		Bhvr	behaviors/Crystalprojectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:CrystalStreaksRed.part
		Part	:CrystalStreaksRed.part
		Part1	:CrystalStreaksRed.part
		Part2	:CrystalGlowsRed.part
		Part4	:CrystalStarsRed.part
		Sound RedToss 90 90 .8
		
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End


End			