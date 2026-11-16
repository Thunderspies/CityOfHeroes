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
		Type	Local 
		At	head
		bhvr	:Magmite_FlamingBreathProjectile.bhvr
		Geom	LocalPivot01
		#Lookat	Target

	End


	Event
		Type	Local 
		At	Prime
		altpiv	2
			
		Sound flamethrower 120.0 120.0 .95
		
		Part1	WeaponFx/BehemothFlameThrowerFlame.part		
		Part	WeaponFx/BehemothFireSpecksSpit.part
		Part	WeaponFx/BehemothFireSpecksFalling.part
		Part	WeaponFx/BehemothFireSpit2.part
		Part	WeaponFx/BehemothFireSpitStart.part
	
		Part	WEAPONFX/BehemothTorchflamethrowerA.part
			
		Part	WeaponFx/BehemothTorchflamethrowerB.part
		bhvr	:Magmite_FlamingBreathOffset.bhvr
		
		LifeSpan	40
	End
End



End