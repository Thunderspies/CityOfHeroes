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
		Geom	Green_CrystalProjectile
		Bhvr	behaviors/Crystalprojectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:CrystalStreaksGreen.part
		Part	:CrystalStreaksGreen.part
		Part1	:CrystalStreaksGreen.part
		Part2	:CrystalGlowsGreen.part
		Part4	:CrystalStarsGreen.part
		Sound GreenToss 90 90 .8
		
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