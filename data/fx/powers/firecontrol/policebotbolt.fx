#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End    

Input  
	Inpname	Mystic
End

LifeSpan	70

Condition
	Event
		EName 	SoundTravelling
		Type	Start 
		At	Mystic
		Bhvr	behaviors/Fastprojectile4.bhvr
		Magnet	Target
		LookAt	Target
		Sound policelaser 70 70 .73
	End

	Event
		EName 	TheBeam
		Type	Start 
		At	Mystic
		Bhvr	behaviors/PoliceBotBeam.bhvr
		geom	PoliceBotBlast1
		Magnet	Target
		LookAt	Target
		LifeSpan 40
	End

	Event
		EName	Blastcone
		Type	posit
		At	Target
		Bhvr	behaviors/PoliceBotBlastHeadInner.bhvr
		geom	PoliceBotBlastCone1
		Part1	:PoliceBotWhiteFlame.part
		Magnet	Origin
		LookAt	Origin
		LifeSpan 28
	End
End

End	


			