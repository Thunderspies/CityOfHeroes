#########################################################
##	
##

FxInfo


Lifespan	110


Condition
	On 	Time  
	Time 	0
	
	Event
		EName	Torch
		Type	Local
		At	WepR		
		Geom	WaterBackPackSwitch
		Bhvr	behaviors\fadeout.bhvr
		BhvrOverride
			PositionOffset	0 0 .1
			PyrRotate	0 90 -90
		End
	End

End



Condition
	On 	Time  
	Time 	18

	Event
		EName 	flame
		Type	Local 
		At	Torch
		Part1	WORLD/Lairs/cot/Candleflame_sticky.part
		BhvrOverride
			PositionOffset	0 .3 0
			PyrRotate	0 0 0
		End
		LifeSpan	70
						
	End
End



Condition
	On 	Time
	Time 	30

	Event
		EName	Torchbreath
		Type	Local
		At	WepR
		bhvr	behaviors/scale1a.bhvr	
		BhvrOverride
			PositionOffset	0 0 .4
		End
	End

	Event
		Type	Posit
		At	Torchbreath
		Part1	ENEMYFX\Carnival_Of_Shadows\Fire_Breath\FireSpit.part
		Part2	ENEMYFX\Carnival_Of_Shadows\Fire_Breath\FireSpecksSpit.part
		Part3	ENEMYFX\Carnival_Of_Shadows\Fire_Breath\FireSpecksFalling.part
		Part4	ENEMYFX\Carnival_Of_Shadows\Fire_Breath\FireSpit2.part
		Part5	ENEMYFX\Carnival_Of_Shadows\Fire_Breath\FireSpitStart.part
		Magnet	Target
		LookAt	Target
		Sound arcoflame 100 100 .88
		Lifespan 50
		
	End
End

End				