#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	100

Input  
	InpName	Root
End    




Condition
	On	Time
	Time	48

	Event
		Type	local
		At	Origin
		Sound ghostblast 90 90 .8

	End
End


Condition
	On 	Time
	Time 	15
	
	Event
		Type	local
		At	wepL

		BHVR	:CrystalOffsetScale.bhvr
		Geom	Keldian_BlackCrystal
		Sound suckup 80 80 .5
		LifeSpan	60		
		
	End

	Event
		Type	local
		At	wepL


		Part	:SoulCrystal01.part
		Part	:SoulCrystalLightRays.part
		
		LifeSpan	50		
		
	End
End    


End			