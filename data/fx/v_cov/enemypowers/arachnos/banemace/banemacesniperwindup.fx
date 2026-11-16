#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End    

Input  
	Inpname	Head
End


#########################################################

Condition
	On 	Time
	Time 	24
	
	Event
		Type	Local
		At 	origin
		Sound cock2 60 60 .46
	End
End


Condition
	On 	Time
	Time 	30
	
	Event
		Type	Local
		At 	origin
		Sound rumble7 60 60 .3
	End
End


Condition
	On 	Time
	Time 	140
	
	Event
		Type	Local
		At 	origin
		Sound maceramp 60 60 .45
	End
End





Condition
	On 	Time
	Time 	0
	
	Event
		EName	Point
		Type	local	
		At	wepR
	
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Geom	heavyMace
	End	

	Event
		EName	Prime
		Type	Local
		At	Point
		Altpiv	2

		#Bhvr	:Projectile1.bhvr
		Magnet	Prime
		LookAt	Prime
		
		
		Part	:EnergyHeadBuildUP.part
		Part	:RapidCoreLocal.part
		Part	:RapidCoreLocalInvert.part
		
		Part	:BotTrailBuildUP.part
		Part	:BotTrailSpecksBuildUP.part
		Part	:BotBubblesBuildUP.part
		#Part	:BotBallzTrail.part
		Sound BaneMaceCharge_loop 80 80 .7
		
	End

	
End

#Condition
#	On 	Time
#	Time	60
#
#	Event
#		EName 	Prime
#		Type	Destroy
#	End
#
#	Event
#		Type	Start
#		At	Point
#		Altpiv	2
#	
#		ChildFX	:BaneMaceSniperBlast.fx
#	End
#
#End
#
End				