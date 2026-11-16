#########################################################
##	
##

FxInfo
LifeSpan  100

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	T_Chest
End

Input  
	InpName	Mystic
End

Input  
	InpName	BendMystic
End

Input  
	InpName	WepL
End

Lifespan	15

##################################
Condition
	On 	Time
	Time 	1




	Event
		EName 	Prime
		Type	Local
		At	Waist
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missile
		Part1	AnimatedCharacterParts/5thmissile_smoke.part
		Part2	AnimatedCharacterParts/5thmissile_smoke2.part
		Part3	AnimatedCharacterParts/5thmissile_fire.part
	End


End



End				