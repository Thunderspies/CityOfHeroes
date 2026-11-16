#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	100


#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	GeomRing
		Type	local 
		At	Root
		Geom	RootToChestAdjustment
						
	End

	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		Part1	:Caleb_DarkBlastTenticle.part
		Part3	:Caleb_DarkHandsEmberb.part
		Sound blackstar1 100 100 .8
		
		PMagnet	LarmR
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	:Caleb_DarkBlastTenticle.part
		Part3	:Caleb_DarkHandsEmberb.part
				
		PMagnet	LarmL
	End

End

Condition
	On 	Time
	Time 	49

	Event
		EName 	Rings
		Type	start 
		At	GeomRing
		AltPiv	1
		
		Part1	:BlackStarRing.Part
		LifeSpan	40
	End

End

Condition
	On 	Time
	Time 	55

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	:Caleb_DarkBlastTenticle.part
		Part3	:Caleb_DarkHandsEmberb.part
		
		PMagnet	WepR
	End

	

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	:Caleb_DarkBlastTenticle.part
		Part3	:Caleb_DarkHandsEmberb.part
		
		PMagnet	WepL
	End

End


Condition
	On 	Time
	Time	50

	Event
		EName 	HitFake
		Type	local 
		At	chest
		Bhvr	Behaviors/CameraShakeDarkCast.bhvr
		Part1	:Blast.part
		Part2	:Blast.part
		Part3	:BlackStarCore.part
		#Part4	:CreateDarknessPuddle2.part
						
	End

	Event
		EName 	HookupPoint
		Type	start 
		At	GeomRing
		AltPiv	1
		
		Bhvr 	Behaviors/DarkScale5.bhvr
		Geom	FX_DarkWave
		LifeSpan	8
	End

	Event
		EName 	HookupPoint
		Type	start 
		At	GeomRing
		AltPiv	1
		
		Bhvr 	Behaviors/DarkScale_5.bhvr
		Geom	FX_DarkWave
		LifeSpan	8
	End

	Event
		EName 	HookupPoint
		Type	start 
		At	GeomRing
		AltPiv	1
		
		Bhvr 	Behaviors/DarkScale_a5.bhvr
		Geom	FX_DarkWave
		LifeSpan	8
	End

	End

Condition
	On 	Time
	Time	63

	Event
		EName 	HitFake
		Type	Destroy

	End
End

Condition
	On 	Time
	Time	45

	Event
		EName 	Hand1
		Type	Destroy

	End

	Event
		EName 	Hand2
		Type	Destroy

	End

	Event
		EName 	Hand3
		Type	Destroy

	End

	Event
		EName 	Hand4
		Type	Destroy

	End

End


End	


			