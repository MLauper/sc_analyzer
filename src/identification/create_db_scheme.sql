
USE [sc_analyzer]
GO

/* DROP TABLES if they exist */

IF OBJECT_ID('dbo.siftKeypoints', 'U') IS NOT NULL DROP TABLE [dbo].[siftKeypoints]
GO

IF OBJECT_ID('dbo.surfKeypoints', 'U') IS NOT NULL DROP TABLE [dbo].[surfKeypoints]
GO

IF OBJECT_ID('dbo.tracks', 'U') IS NOT NULL DROP TABLE [dbo].[tracks]
GO

IF OBJECT_ID('dbo.cameras', 'U') IS NOT NULL DROP TABLE [dbo].[cameras]
GO

IF OBJECT_ID('dbo.persons', 'U') IS NOT NULL DROP TABLE [dbo].[persons]
GO

/* CREATE TABLES */

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[cameras](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[scene] [int] NOT NULL,
	[directory] [varchar](1000) NOT NULL,
	[prefix] [varchar](200) NOT NULL,
	[width] [int] NOT NULL,
	[height] [int] NOT NULL,
	[fps] [int] NOT NULL,
	[pixelToCentimeterRatio] [float] NOT NULL,
 CONSTRAINT [PK_cameras] PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[persons](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[person_id] [int] NOT NULL,
 CONSTRAINT [PK_persons] PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO


SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[tracks](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[camera_id] [int] NOT NULL,
	[trackId] [int] NOT NULL,
	[walkingDirection] [varchar](50) NOT NULL,
	[primaryColor_UpperBody] [int] NOT NULL,
	[primaryColor_LowerBody] [int] NOT NULL,
	[personSize_height] [float] NOT NULL,
	[personSize_width] [float] NOT NULL,
	[person_id] [int] NULL,
 CONSTRAINT [PK_tracks] PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[tracks]  WITH CHECK ADD  CONSTRAINT [FK_tracks_cameras] FOREIGN KEY([camera_id])
REFERENCES [dbo].[cameras] ([id])
ON UPDATE CASCADE
ON DELETE CASCADE
GO

ALTER TABLE [dbo].[tracks] CHECK CONSTRAINT [FK_tracks_cameras]
GO

ALTER TABLE [dbo].[tracks]  WITH CHECK ADD  CONSTRAINT [FK_tracks_persons] FOREIGN KEY([person_id])
REFERENCES [dbo].[persons] ([id])
GO

ALTER TABLE [dbo].[tracks] CHECK CONSTRAINT [FK_tracks_persons]
GO

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[siftKeypoints](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[x] [float] NOT NULL,
	[y] [float] NOT NULL,
	[size] [float] NOT NULL,
	[angle] [float] NOT NULL,
	[response] [float] NOT NULL,
	[octave] [int] NOT NULL,
	[class_id] [int] NOT NULL,
	[track_id] [int] NOT NULL,
 CONSTRAINT [PK_siftKeypoints] PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[siftKeypoints]  WITH CHECK ADD  CONSTRAINT [FK_siftKeypoints_tracks] FOREIGN KEY([track_id])
REFERENCES [dbo].[tracks] ([id])
ON UPDATE CASCADE
ON DELETE CASCADE
GO

ALTER TABLE [dbo].[siftKeypoints] CHECK CONSTRAINT [FK_siftKeypoints_tracks]
GO

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[surfKeypoints](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[x] [nchar](10) NOT NULL,
	[y] [nchar](10) NOT NULL,
	[size] [float] NOT NULL,
	[angle] [float] NOT NULL,
	[response] [float] NOT NULL,
	[octave] [int] NOT NULL,
	[class_id] [int] NOT NULL,
	[track_id] [int] NOT NULL,
 CONSTRAINT [PK_surfKeypoints] PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[surfKeypoints]  WITH CHECK ADD  CONSTRAINT [FK_surfKeypoints_tracks] FOREIGN KEY([track_id])
REFERENCES [dbo].[tracks] ([id])
ON UPDATE CASCADE
ON DELETE CASCADE
GO

ALTER TABLE [dbo].[surfKeypoints] CHECK CONSTRAINT [FK_surfKeypoints_tracks]
GO

