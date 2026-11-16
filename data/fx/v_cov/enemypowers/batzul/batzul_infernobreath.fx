#########################################################
##	
##

FxInfo
LifeSpan  200

##################################
Condition
	On 	Time
	Time 	36

	Event
		EName 	Prime
		Type	local 
		At	head
		bhvr	behaviors/scale1a.bhvr
		Bhvroverride
			pyrRotate	-10 0 0
		End
		Geom	LocalPivot01

	End


	Event
		Type	local 
		At	Prime
		altpiv	2
		Bhvroverride
			PositionOffset	0 .5 0
		End
			
		Sound flamethrower2 120.0 120.0 .95
		
		Part1	:BehemothFlameThrowerFlame.part		
		Part	:BehemothFireSpecksSpit.part
		Part	:BehemothFireSpecksFalling.part
#		Part	:BehemothFireSpit2.part
		Part	:BehemothFireSpitStart.part
	
		Part	:BehemothTorchflamethrowerA.part
			
		Part	:BehemothTorchflamethrowerB.part

		LifeSpan	40
	End

End



End