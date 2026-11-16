#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	Root
End

Input  
	InpName	Head
End

Input  
	InpName	Chest
End

Input  
	InpName	Hips
End

##################################
Condition
	On	Time
	Time	0
	
	Event
		Type	Local 
		At	Root
		#part	:GlideCloudTrailMist.part
		#part	:GlideCloudTrail.part
			#part	:GlideCloudFlatLocalTrail.part
			#part	:GlideCloudTrailFlat.part
			#part	:GlideCloudFlatLocal.part
		#part	:GlideMist.part
		#part	:GlideCloud.part
		#part	:SpeedStreaks.part
		Sound Green_loop 30 30 .15
		               
	End
		
	Event
		Type	Local 
		At	FootL
		part	:GlowStreaks.part
		part	:GlideCloudTrailMist.part
		part	:GlideCloudTrail.part
		part	:GlideCloudFlatLocalTrail.part
		part	:GlideCloudTrailFlat.part
		part	:GlideCloudFlatLocal.part
		part	:GlideMist.part
		part	:GlideCloud.part
		#part	:SpeedStreaks.part
		part	:GlowS.part
		               
	End

	Event
		Type	Local 
		At	FootR
		part	:GlowStreaks.part
		part	:GlideCloudTrailMist.part
		part	:GlideCloudTrail.part
		part	:GlideCloudFlatLocalTrail.part
		part	:GlideCloudTrailFlata.part
		part	:GlideCloudFlatLocal.part
		part	:GlideMist.part
		part	:GlideCloud.part
		#part	:SpeedStreaks.part
		part	:GlowS.part
		               
	End

End

End				