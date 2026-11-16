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
		Geom	Charcoal_CrystalProjectile
		Bhvr	behaviors/Crystalprojectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:CrystalStreaksCharcoal.part
		Part	:CrystalStreaksCharcoal.part
		Part1	:CrystalStreaksCharcoal.part
		Part2	:CrystalGlowsCharcoal.part
		Part4	:CrystalStarsCharcoal.part
		Sound BlackToss 90 90 .8
		
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