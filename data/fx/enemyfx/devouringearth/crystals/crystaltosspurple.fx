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
		Geom	Purple_CrystalProjectile
		Bhvr	behaviors/Crystalprojectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:CrystalStreaksPurple.part
		Part	:CrystalStreaksPurple.part
		Part1	:CrystalStreaksPurple.part
		Part2	:CrystalGlowsPurple.part
		Part4	:CrystalStarsPurple.part
		Sound PurpleToss 90 90 .75
		
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