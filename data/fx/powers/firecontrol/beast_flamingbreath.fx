#########################################################
##	
##

FxInfo
LifeSpan  200

Input  
	InpName	Target
End

Input  
	InpName	head
End

Input  
	InpName	WepR
End

##################################
Condition
	On 	Time
	Time 	39

	Event
		EName 	Prime
		Type	local 
		At	head
		bhvr	behaviors/scale1a.bhvr
		Geom	LocalPivot01

	End


	Event
		Type	local 
		At	Prime
		altpiv	2
			
		Sound flamethrower2 120.0 120.0 .95
		
		Part1	WeaponFx/BehemothFlameThrowerFlame.part		
		Part	WeaponFx/BehemothFireSpecksSpit.part
		Part	WeaponFx/BehemothFireSpecksFalling.part
		Part	WeaponFx/BehemothFireSpit2.part
		Part	WeaponFx/BehemothFireSpitStart.part
	
		Part	WEAPONFX/BehemothTorchflamethrowerA.part
			
		Part	WeaponFx/BehemothTorchflamethrowerB.part

		LifeSpan	45
	End

End



End