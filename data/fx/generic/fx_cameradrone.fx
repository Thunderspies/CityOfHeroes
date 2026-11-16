#########################################################
##	template

FxInfo

Condition

	On 	Time
	Time 	0

	Event
		Type	Local
		At	origin
		Sound policedrone2_loop 100 100 .18
	End
End


Condition
	On	time
	Time 	0


	Event
		Ename	leftLampBlue
		Type	Local
		At	FootR
		BHVR	:CameraLightOffsetRight.bhvr
		Part1	:AmberLightCenter2.part
		Part2	:AmberLightGlow1.part
		Part3	:AmberLightGlow2.part
		Part4	:AmberLightRing.part
		

	End

End

Condition
	On	time
	Time 	15

	Event
		Ename	leftLampBlue
		Type	Local
		At	FootL
		BHVR	:CameraLightOffset.bhvr
		Part1	:AmberLightCenter2.part
		Part2	:AmberLightGlow1.part
		Part3	:AmberLightGlow2.part
		Part4	:AmberLightRing.part

	End

	
End

Condition
	On 	Time
	Time 	15

	Event
		Ename	TopRightLamp
		Type	Local
		At	head
		
		Part1	:CameraAntennaSpark.part
		#Part2	:RedLightGlow.part

	End

End

End		